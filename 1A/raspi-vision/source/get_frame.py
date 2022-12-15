import numpy as np
import cv2
from camera_info import *
from render_scene_gl import Renderer
import trigo
import cylindre_obj

if CameraConstants.OnPi:
    from picamera.array import PiRGBArray
    from picamera import PiCamera

class FakeFrame:
    def __init__(self, *args):
        texture_image = cv2.imread("../data/spritesheet.png")
        texture_image = cv2.cvtColor(texture_image, cv2.COLOR_BGR2RGB)
        self.cap = Renderer(texture_image, "../data/shader.vert", "../data/shader.frag")
        self.is_fake = True

        self.cam_update = True
        self.data_update = False
        
        self.cam_pitch = CameraInfo.AngleHorizontal
        self.cam_ang_bounds = CameraInfo.AngleHorizontalLimites
        self.cam_ang_speed = 1
        final_y = TableConstants.Profondeur // 2
        final_x = TableConstants.Largeur // 2
        self.cam_speed = 50
        self.cam_side_order = "WSEN"
        self.cam_side_current = 0
        self.cam_yaw = CameraInfo.AngleVertical
        self.cam_yaw_arr = {k: v for k, v in [
            ("S", 0),
            ("W", 90),
            ("N", 180),
            ("E", -90),
        ]}
        self.cam_bounds_pos = {k: np.array(v, dtype=int) for k, v in [
            ("S", (final_x, 0, 0)),
            ("W", (0, 0, final_y)),
            ("N", (final_x, 0, 0)),
            ("E", (0, 0, final_y)),
        ]}
        self.cam_dir_order = {k: np.array(v, dtype=int) for k, v in [
            ("S", (-1,  0,  0)),
            ("W", ( 0,  0, -1)),
            ("N", ( 1,  0,  0)),
            ("E", ( 0,  0,  1)),
        ]}
        self.cam_pos = np.array([CameraInfo.Lateral, CameraInfo.Elevation, CameraInfo.Profondeur], dtype=int)
        self.next_cam_bound(+1)

        self.possible_kinds_vbo_vaos = []
        for kind in range(3):
            to_append_uv = [(tex[0], 1.0 - (tex[1] - (kind * (0.25)))) for tex in cylindre_obj.uv]
            self.possible_kinds_vbo_vaos.append(self.cap.prepareVBOVAO(cylindre_obj.vtx, to_append_uv, cylindre_obj.normal))

    def read(self, img):
        if self.cam_update:
            CameraInfo.AngleVertical = self.cam_yaw
            CameraInfo.AngleHorizontal = self.cam_pitch
            CameraInfo.Lateral = self.cam_pos[0]
            CameraInfo.Elevation = self.cam_pos[1]
            CameraInfo.Profondeur = self.cam_pos[2]
            self.cap.prepareCamera()
            self.cam_update = False

        if self.data_update:
            self.cap.finalizeVBOVAO(self.cap.getObjectCount(), self.to_add_vbo, self.to_add_vao, self.to_add_mat)
            self.data_update = False

        return self.cap.read(img)

    def release(self):
        pass

    def add_data(self, kind, angle, position):
        self.to_add_vbo, self.to_add_vao = self.possible_kinds_vbo_vaos[kind]
        self.to_add_mat = trigo.creer_matrice_modele(position, [0, angle, 0], [1.0] * 3)
        self.data_update = True

    def next_cam_bound(self, direction):
        self.cam_side_current = (self.cam_side_current + len(self.cam_side_order) + direction) % len(self.cam_side_order)
        side = self.cam_side_order[self.cam_side_current]
        self.cam_yaw = self.cam_yaw_arr[side]
        self.cam_dir = self.cam_dir_order[side]
        self.cam_bound_upper = self.cam_bounds_pos[side]
        self.cam_bound_lower = -self.cam_bound_upper
        self.cam_update = True

    def get_world_space_proj(self, x, y):
        x = CameraConstants.SensorW - x - 1
        z = self.cap.getDepthAtPoint(x, y)
        if z is None:
            return None

        return trigo.untransform_point(x,y,z)

class RealFrame:
    def __init__(self, cam_id=0) -> None:
        self.cap = cv2.VideoCapture(cam_id)
        self.is_fake = False
        CameraConstants.SensorW = 1280
        CameraConstants.SensorH = 720

        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, CameraConstants.SensorW)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, CameraConstants.SensorH)
        
        CameraConstants.SensorW = int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        CameraConstants.SensorH = int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
        print("W/H:", CameraConstants.SensorW, CameraConstants.SensorH)

    def read(self, img):
        return self.cap.read(img)

    def release(self):
        self.cap.release()

class RealPiFrame:
    def __init__(self):
        self.camera = PiCamera()
        self.camera.resolution = (CameraConstants.SensorW, CameraConstants.SensorH)
        self.camera.framerate = 12
        self.is_fake = False

    def read(self, img):
        success = self.camera.capture(img, format="bgr")
        return success, img

    def release(self):
        self.camera.close()

