from camera_info import *

vtx = [
    [-TableConstants.Largeur / 2.0, 0,  TableConstants.Profondeur / 2.0],
    [ TableConstants.Largeur / 2.0, 0, -TableConstants.Profondeur / 2.0],
    [-TableConstants.Largeur / 2.0, 0, -TableConstants.Profondeur / 2.0],
    [-TableConstants.Largeur / 2.0, 0,  TableConstants.Profondeur / 2.0],
    [ TableConstants.Largeur / 2.0, 0,  TableConstants.Profondeur / 2.0],
    [ TableConstants.Largeur / 2.0, 0, -TableConstants.Profondeur / 2.0],
]
uv = [
    [0.5, 0],
    [0, 1],
    [0.5, 1],
    [0.5, 0],
    [0, 0],
    [0, 1],
]
normal = [
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
]