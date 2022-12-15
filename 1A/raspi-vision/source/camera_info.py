import numpy as np

class TableConstants:
    Largeur = 2000 # mm
    Profondeur = 3000 # mm
    Centre = (0, 0)
    TagCenterOffset = (570, 575)
    TagSideSize = 100
    TagsPositions = {
        20: np.array((-(Largeur//2 - TagCenterOffset[0]), 0,  (Profondeur//2 - TagCenterOffset[1])), dtype=int),
        21: np.array(( (Largeur//2 - TagCenterOffset[0]), 0,  (Profondeur//2 - TagCenterOffset[1])), dtype=int),
        22: np.array((-(Largeur//2 - TagCenterOffset[0]), 0, -(Profondeur//2 - TagCenterOffset[1])), dtype=int),
        23: np.array(( (Largeur//2 - TagCenterOffset[0]), 0, -(Profondeur//2 - TagCenterOffset[1])), dtype=int),
    }
    EpaisseurCouche = 20
    RayonCouche = 60
    DistanceMilieuArucoCentreCouche = 33

class CameraInfo:
    Elevation = 1500 # mm
    Lateral = 0 # mm
    Profondeur = -(TableConstants.Profondeur // 2) # mm

    AngleVertical = 0
    AngleHorizontal = 24

    ElevationLimites = (200, 2000)
    AngleHorizontalLimites = (0, 80)

class CameraConstants:
    FovH = 62.2 # deg
    FovV = 48.8 # deg
    
    # SensorW = 3280
    # SensorH = 2464

    Xscale = 0.9375
    Yscale = 0.9375

    SensorW = 1640 # px
    SensorH = 1232 # px

    Focale = 3.04 # mm
    OnPi = False

    Znear = 0.01
    Zfar = 10.0
