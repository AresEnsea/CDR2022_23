from pyrr import Matrix44, Matrix33
from pyrr.matrix44 import apply_to_vector
import numpy as np
from camera_info import *
import cv2

class Valeurs_Classe:
    @property
    def Perspective(self):
        return Matrix44.perspective_projection(CameraConstants.FovV, CameraConstants.SensorW / CameraConstants.SensorH, CameraConstants.Znear, CameraConstants.Zfar)

    @property
    def CameraPosition(self):
        """
        Vecteur 3D de position
        En millimètres
        """
        return np.array([CameraInfo.Lateral, CameraInfo.Elevation, CameraInfo.Profondeur])

    @property
    def CameraRotation(self):
        """
        Matrice 3x3 de rotation 3D
        applicable à des vecteurs 3D
        """
        return Matrix33.from_y_rotation(np.deg2rad(CameraInfo.AngleVertical)) * \
            Matrix33.from_x_rotation(np.deg2rad(CameraInfo.AngleHorizontal + 90))

    @property
    def MatriceView(self):
        matPos = self.CameraPosition

        camMat = Matrix44.from_translation(matPos * 1e-3) * \
            Matrix44.from_y_rotation(np.deg2rad(CameraInfo.AngleVertical)) * \
            Matrix44.from_x_rotation(np.deg2rad(CameraInfo.AngleHorizontal + 90))

        return camMat.inverse

    @property
    def MatriceVP(self):
        return self.Perspective * self.MatriceView

Valeurs = Valeurs_Classe()

def creer_matrice_modele(position, rotation, echelle):
    return Matrix44.from_translation(position) * \
        Matrix44.from_z_rotation(np.deg2rad(rotation[2])) * \
        Matrix44.from_y_rotation(np.deg2rad(rotation[1])) * \
        Matrix44.from_x_rotation(np.deg2rad(rotation[0])) * \
        Matrix44.from_scale(echelle)

def distance(vec):
    return np.linalg.norm(vec)

def distance_entre(a, b):
    return distance(a - b)

def cam_dist_vers_z(distance_from_camera):
    far_z = CameraConstants.Zfar
    near_z = CameraConstants.Znear
    # world_z = 2 * far_z * near_z / (clip_z * (far_z - near_z) - ( far_z + near_z))
    # 2 * far_z * near_z / world_z = clip_z * (far_z - near_z) - ( far_z + near_z)
    # clip_z * (far_z - near_z) = (2 * far_z * near_z / world_z) + ( far_z + near_z)
    clip_z = (2 * far_z * near_z / distance_from_camera) / (far_z - near_z) + 1.0
    return clip_z

def untransform_point(x, y, z):
    ndcPos = np.array([
        (2.0 * (x / CameraConstants.SensorW) - 1.0),
        (2.0 * (y / CameraConstants.SensorH) - 1.0),
        z,
        1.0
    ])

    point = apply_to_vector(Valeurs.MatriceVP.inverse, ndcPos)
    point_one_w = 1.0 / point[3]
    point *= point_one_w

    return point[0:3]

def trouverPointSelonHauteurFixe(x, y, hauteur):
    near_pt = untransform_point(CameraConstants.SensorW - x - 1, y, -1.0) * 1e3
    far_pt = untransform_point(CameraConstants.SensorW - x - 1, y, +1.0) * 1e3
    # near_pt + k * (far_pt - near_pt) = actual_pt
    # k reel, reste vecteurs
    # near_pt.y + k * (far_pt.y - near_pt.y) = actual_pt.y
    # actual_pt.y = EpaisseurCouche
    # --> k = (EpaisseurCouche - near_pt.y) / (far_pt.y - near_pt.y)
    k = (hauteur - near_pt[1]) / (far_pt[1] - near_pt[1])
    return near_pt + k * (far_pt - near_pt)

