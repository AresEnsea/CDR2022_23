import moderngl as ModernGL
from pyrr import Matrix44
from pyrr.matrix44 import apply_to_vector
import struct
import numpy as np
from camera_info import *
import trigo
import table_obj
import table_porte_cerise
import cerise_obj

class Renderer:
    def __init__(self, texture_image, vert_path, frag_path) -> None:
        vertex_shader_source = open(vert_path).read()
        fragment_shader_source = open(frag_path).read()
        self.ctx = ModernGL.create_standalone_context()

        self.prog = self.ctx.program(vertex_shader=vertex_shader_source, fragment_shader=fragment_shader_source)
        self.prog['Color'].value = (1.0, 1.0, 1.0, 0.125)

        self.texture = self.ctx.texture(texture_image.shape[1::-1], texture_image.shape[2], texture_image)
        self.texture.build_mipmaps()

        self.fbo = self.ctx.simple_framebuffer((CameraConstants.SensorW, CameraConstants.SensorH))

        self.pixel_bytes = bytearray(CameraConstants.SensorW * CameraConstants.SensorH * 3)
        self.objects = {}
        self.prepareVtxUV("table", table_obj.vtx, table_obj.uv, table_obj.normal, trigo.creer_matrice_modele(
            [0] * 3,
            [0] * 3,
            [1e-3] * 3
        ))

        porte_cerise_noms = (
            "porte_cerise_W",
            "porte_cerise_E",
            "porte_cerise_N",
            "porte_cerise_S",
        )
        porte_cerise_positions = (
            np.array([-(1000), -30, 0]),
            np.array([(1000 - 30), -30, 0]),
            np.array([-15, -30, (1500 - 150)]),
            np.array([-15, -30, -(1500 - 150)]),
        )
        porte_cerise_mats = (trigo.creer_matrice_modele(p * 1e-3, [90, 0, 0], [1e-3] * 3) for p in porte_cerise_positions)
        self.prepareVtxUV(porte_cerise_noms, table_porte_cerise.vtx, table_porte_cerise.uv, table_porte_cerise.normal, porte_cerise_mats)

        cerise_noms = (f"cerise_{idx}" for idx in range(4*10))
        cerise_mats = []
        start_points_cerises = (
            [0, 30, (1500 - 15)],
            [(1000 - 15), 30, (300 // 2 - 15)],
            [-(1000 - 15), 30, (300 // 2 - 15)],
            [0, 30, -(1500 - 300 + 15)],
        )
        for cerise_pos in start_points_cerises:
            cerise_pos = np.array(cerise_pos)
            for j in range(10):
                cerise_mats.append(trigo.creer_matrice_modele(cerise_pos * 1e-3, [0, 0, 0], [11.25e-3] * 3))
                cerise_pos += [0, 0, -30]

        self.prepareVtxUV(cerise_noms, cerise_obj.vtx, ((a[0], 1.0 - a[1]) for a in cerise_obj.uv), cerise_obj.normal, cerise_mats)

    def getObjectCount(self) -> int:
        return len(self.objects) - 1

    def finalizeVBOVAO(self, name, vbo, vao, mat):
        if (not hasattr(name, '__iter__')) or (type(name) is str):
            name = (name,)
            mat = (mat,)

        for nam, modmat in zip(name, mat):
            self.objects[nam] = (modmat, vbo, vao)

    def prepareVBOVAO(self, vertex_data, uv_data, normal_data):
        data = b"".join(map(lambda x,y,z: struct.pack("3f2f3f", *x, *y, *z), vertex_data, uv_data, normal_data))
        vbo = self.ctx.buffer(data)
        vao = self.ctx.simple_vertex_array(self.prog, vbo, * ['in_vert', 'in_text', 'in_norm'])
        return vbo, vao

    def prepareVtxUV(self, name, vertex_data, uv_data, normal_data, mat) -> None:
        vbo, vao = self.prepareVBOVAO(vertex_data, uv_data, normal_data)
        self.finalizeVBOVAO(name, vbo, vao, mat)

    def prepareCamera(self) -> None:
        self.prog['Light'].value = trigo.Valeurs.CameraPosition * 1e-3
        self.prog['MatVP'].write(trigo.Valeurs.MatriceVP.astype('f4').tobytes())

    def read(self, img):
        self.fbo.use()
        self.ctx.enable(ModernGL.DEPTH_TEST)
        self.ctx.enable(ModernGL.CULL_FACE)
        self.ctx.clear(0.0, 0.0, 0.0)
        self.texture.use()
        for (modelmat, vbo, vao) in self.objects.values():
            self.prog['Model'].write(modelmat.astype('f4').tobytes())
            vao.render()

        self.fbo.read_into(self.pixel_bytes)
        deserialized_bytes = np.frombuffer(self.pixel_bytes, dtype=np.uint8)
        deserialized_x = np.reshape(deserialized_bytes, newshape=(CameraConstants.SensorH, CameraConstants.SensorW, 3))
        np.copyto(img, np.flip(deserialized_x, (1,2)))
        return True, img

    def getDepthAtPoint(self, x, y):
        far_z = CameraConstants.Zfar
        near_z = CameraConstants.Znear

        # valdepths = np.frombuffer(self.fbo.read(attachment=-1, dtype='f4'), dtype=np.dtype('f4')).reshape((CameraConstants.SensorH, CameraConstants.SensorW)).copy()
        valdepths = np.frombuffer(self.fbo.read(attachment=-1, viewport=(x, y, 1, 1), dtype='f4'), dtype=np.dtype('f4'))[0]
        clip_z = (valdepths - 0.5) * 2.0
        world_z = 2 * far_z * near_z / (clip_z * (far_z - near_z) - ( far_z + near_z))

        if world_z < -CameraConstants.Zfar or world_z > -CameraConstants.Znear:
            return None

        # depth = clip_z[y, x]
        depth = clip_z
        return depth
