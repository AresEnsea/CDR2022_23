import numpy as np
import cv2
import cv2.aruco as aruco
from camera_info import *
import display_frame as Display
import get_frame as Getter
import trigo

def findArucoMarkers(img, markerSize=4, totalMarkers=100):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    key = getattr(aruco, f'DICT_{markerSize}X{markerSize}_{totalMarkers}')
    arucoDict = aruco.Dictionary_get(key)
    arucoParam = aruco.DetectorParameters_create()
    bboxs, ids, rejected = aruco.detectMarkers(gray, arucoDict, parameters = arucoParam)
    return [bboxs, ids]

def findCerises(img):
    blurred = img
    blurred = cv2.medianBlur(img, 3)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    low_hue = cv2.inRange(hsv, (0, 140, 140), (8, 255, 255))
    high_hue = cv2.inRange(hsv, (172, 140, 140), (180, 255, 255))
    fin = cv2.addWeighted(low_hue, 1.0, high_hue, 1.0, 0.0)
    fin = cv2.GaussianBlur(fin, (5,5), 2, 2)
    ret = cv2.HoughCircles(fin, cv2.HOUGH_GRADIENT,
        dp=1.0,
        minDist=24,
        param1=50,
        param2=10,
        minRadius=8,
        maxRadius=25)
    return [((r[0], r[1]), r[2]) for r in ret[0]] if ret is not None and len(ret) != 0 else None

known_tags =  {
    20: "proche de: aire depose bleu",
    21: "proche de: aire depose vert",
    22: "proche de: fouet",
    23: "proche de: cuillere et raisins",
    47: "glacage (rose)",
    13: "creme (jaune)",
    36: "genoise (marron)",
}
# tag -> mm en x, mm en y, origine au centre de la table

AXIS_COLOR = (
    (255, 0, 0), # bleu vers +X
    (0, 0, 255), # rouge vers +Z
    (0, 255, 0), # vert vers +Y
    (160, 160, 160)
)

def augmentAruco(img, bboxs, tag_ids, draw_font = cv2.FONT_HERSHEY_SIMPLEX):
    if len(bboxs) != 0:
        cv2.fillPoly(img, pts=np.floor(bboxs).astype(int), color=(0, 0, 0))
        for bbox1, tag_id1 in zip(bboxs, tag_ids):
            bbox = bbox1[0]
            tag_id = tag_id1[0]
            centroid = bbox.mean(axis=0)
            towards_middle = np.mean([
                bbox[0] + (bbox[1] - bbox[0]) * 0.5,
                bbox[1] + (bbox[0] - bbox[1]) * 0.5,
            ], axis=0)
            cv2.circle(img, (centroid).astype(int), 5, (0, 255, 255), -1)
            cv2.circle(img, (towards_middle).astype(int), 5, (255, 0, 0), -1)
            # (tw, th), _ = cv2.getTextSize(str(tag_id), draw_font, 1.0, 2)
            # centroid[0] -= tw / 2
            # centroid[1] += th / 2
            # cv2.putText(img, str(tag_id), np.floor(centroid).astype(int), draw_font, 1.0, (160, 160, 160), 2, cv2.LINE_AA)

def drawAxes(img, points, thick = 3):
    points = np.floor(points).astype(int)
    point1 = (points[3][0][0], points[3][0][1])
    cv2.circle(img, point1, 5, (255, 255, 255), -1)
    for col, point2 in zip(AXIS_COLOR, map(lambda v: (v[0], v[1]), points[0:3, 0])):
        cv2.line(img, point1, point2, col, thick)

def main():
    # Fake <-> Real pour different comportements
    # Frame: Rendu 3D vs camera physique
    # Display: pas d'affichage vs affichage et commande pour le rendu
    frame_source = Getter.RealFrame()
    disp = Display.RealDisplay()

    try:
        img = np.zeros((CameraConstants.SensorH, CameraConstants.SensorW, 3), dtype=np.uint8)

        last_str = []
        cx = (CameraConstants.SensorW - 1) / 2.0
        cy = (CameraConstants.SensorH - 1) / 2.0
        # fovy = 2 * arctan(h / (2 * fy))
        # fovx = 2 * arctan(w / (2 * fx))
        # <=>
        # tan(fovy / 2) = h / (2 * fy)
        # tan(fovx / 2) = w / (2 * fx)
        # <=>
        # fy = h / (2 * tan(fovy / 2))
        # fx = w / (2 * tan(fovx / 2))
        fx = CameraConstants.SensorW / (2.0 * np.tan(np.deg2rad(CameraConstants.FovH) / 2.0))
        fy = CameraConstants.SensorH / (2.0 * np.tan(np.deg2rad(CameraConstants.FovV) / 2.0))
        matrix_camera = np.array([
            [fx,  0, cx],
            [ 0, fy, cy],
            [ 0,  0,  1]
        ], dtype=float)
        
        axes = (
            (1.0, 0.0, 0.0),
            (0.0, 0.0, 1.0),
            (0.0, 1.0, 0.0),
            (0.0, 0.0, 0.0)
        )

        new_cam_pos = None
        fail_count = 0

        while True:
            success, img = frame_source.read(img)
            if not success:
                fail_count += 1
                if fail_count == 10:
                    break
                continue
            else:
                fail_count = 0
            arucofound = findArucoMarkers(img)
            cerisefound = findCerises(img)
            augmentAruco(img, arucofound[0], arucofound[1])
            if cerisefound is not None:
                # print("found", len(cerisefound), "cerises")
                for pt, rad in cerisefound:
                    cv2.circle(img, np.round(pt).astype(int), int(np.round(rad)), (255, 255, 255), 2)

            cur_str = []
            known_points_2d_dict = {}
            known_points_3d_dict = {}
            known_point_keys = []
            extra_known_points_2d = []
            # loop through all the markers and augment each one
            if len(arucofound[0]) != 0:
                # print("hit")
                for bbox1, tag_id1 in zip(arucofound[0], arucofound[1]):
                    bbox = bbox1[0]
                    tag_id = tag_id1[0]
                    if 51 <= tag_id <= 70:
                        d = "equipe bleue"
                    elif 71 <= tag_id <= 90:
                        d = "equipe verte"
                    elif known_tags.get(tag_id):
                        d = known_tags[tag_id]
                        pos = TableConstants.TagsPositions.get(tag_id)
                        centroid = bbox.mean(axis=0)
                        if pos is not None:
                            k = (tag_id, -1)
                            known_point_keys.append(k)
                            known_points_2d_dict[k] = (centroid)
                            known_points_3d_dict[k] = (pos)
                            # prendre en compte les coins et pas le centre pour améliorer la précision ?
                            # offset_arr = np.array([TableConstants.TagSideSize // 2, 0, TableConstants.TagSideSize // 2])
                            # for ki, direc in zip(range(4), (
                            #     [-1, 1, +1],
                            #     [+1, 1, +1],
                            #     [+1, 1, -1],
                            #     [-1, 1, -1],
                            # )):
                            #     k = (tag_id, ki)
                            #     known_point_keys.append(k)
                            #     known_points_2d_dict[k] = bbox[ki]
                            #     known_points_3d_dict[k] = (pos) + offset_arr * direc
                            #     break
                        else:
                            towards_middle = np.mean([
                                bbox[0] + (bbox[1] - bbox[0]) * 0.5,
                                bbox[1] + (bbox[0] - bbox[1]) * 0.5,
                            ], axis=0)
                            extra_known_points_2d.append((tag_id, len(extra_known_points_2d), centroid, towards_middle))
                    else:
                        d = "inconnu"

                    cur_str.append(f"tag: {tag_id}, {d}")
            cur_str.sort()

            if len(known_points_2d_dict) >= 4:
                known_points_3d = np.array([known_points_3d_dict[k] for k in known_point_keys]).astype(float)
                known_points_2d = np.array([known_points_2d_dict[k] for k in known_point_keys]).astype(float)

                distortion_coeffs = np.zeros((4,1))
                # success, vector_rotation, vector_translation = cv2.solvePnP(known_points_3d, known_points_2d, matrix_camera, distortion_coeffs, flags=cv2.SOLVEPNP_IPPE)
                success, vector_rotation, vector_translation = cv2.solvePnP(known_points_3d, known_points_2d, matrix_camera, distortion_coeffs, flags=0)

                np_rodrigues = np.asarray(vector_rotation[:,:])
                rot_matrix = cv2.Rodrigues(np_rodrigues)[0]

                camera_pos = -rot_matrix.T @ vector_translation
                camera_pos = camera_pos[:,0]
                camera_pos = camera_pos * [-1, -1, +1]

                new_cam_pos = camera_pos

                # draw 3d axes
                endpts = cv2.projectPoints(np.array(axes) * -125, vector_rotation, vector_translation, matrix_camera, distortion_coeffs)[0]
                drawAxes(img, endpts)

                # draw detected center of cake slices
                for ext_id, ext_n, ext_centre, ext_proche_milieu in extra_known_points_2d:
                    actual_aruco_center_pt = trigo.trouverPointSelonHauteurFixe(ext_centre[0], ext_centre[1], TableConstants.EpaisseurCouche)
                    actual_aruco_top_center_pt = trigo.trouverPointSelonHauteurFixe(ext_proche_milieu[0], ext_proche_milieu[1], TableConstants.EpaisseurCouche)
                    delta = actual_aruco_top_center_pt - actual_aruco_center_pt
                    delta /= np.linalg.norm(delta)
                    actual_cake_center_pt = actual_aruco_center_pt + delta * TableConstants.DistanceMilieuArucoCentreCouche

                    final_pt = cv2.projectPoints(actual_cake_center_pt * [-1, -1, +1], vector_rotation, vector_translation, matrix_camera, distortion_coeffs)[0][0][0]
                    cv2.circle(img, final_pt.astype(int), 10, (160, 160, 160), -1)

            if cur_str != last_str:
                last_str = cur_str

                if len(cur_str) != 0:
                    # print("new str")
                    # print(*cur_str, sep='\n')
                    pass

            if new_cam_pos is not None:
                new_cam_pos = None
                if len(known_points_2d_dict) >= 4:
                    p = camera_pos
                    print("Camera calcul:", p.astype(int) * [-1, +1, +1])
                    # p_r = trigo.Valeurs.CameraPosition
                    # p_err = p - p_r
                    # print("Camera calcul:", p.astype(int) * [-1, +1, +1], "reel:", p_r * [-1, +1, +1])
                    # print("vecteur erreur:", p_err, "error dist:", np.linalg.norm(p_err))

            if not disp.show(frame_source, img):
                break

    except KeyboardInterrupt:
        pass

    frame_source.release()
    disp.release()

if __name__ == "__main__":
    main()
