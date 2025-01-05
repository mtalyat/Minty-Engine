using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public enum Perspective : int
    {
        Perspective,
        Orthographic,
    }

    public class Camera : Component
    {
        public Perspective Perspective
        {
            get => (Perspective)Runtime.Camera_GetPerspective(Entity.ID);
            set => Runtime.Camera_SetPerspective(Entity.ID, (int)value);
        }

        public float FOV
        {
            get => Runtime.Camera_GetFov(Entity.ID);
            set => Runtime.Camera_SetFov(Entity.ID, (float)value);
        }

        public float Near
        {
            get => Runtime.Camera_GetNear(Entity.ID);
            set => Runtime.Camera_SetNear(Entity.ID, (float)value);
        }

        public float Far
        {
            get => Runtime.Camera_GetFar(Entity.ID);
            set => Runtime.Camera_SetFar(Entity.ID, (float)value);
        }

        public Color Color
        {
            get => Color.FromInt(Runtime.Camera_GetColor(Entity.ID));
            set => Runtime.Camera_SetColor(Entity.ID, value.ToInt());
        }

        internal Camera()
        { }

        public static void SetMain(Camera camera) => Runtime.Camera_SetMain(camera.Entity.ID);

        public static Camera GetMain() => (Camera)Runtime.Camera_GetMain();
    }
}
