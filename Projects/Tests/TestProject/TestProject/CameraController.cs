using MintyEngine;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestProject
{
    public class CameraController : Script
    {
        private float pitch = 0.0f;
        private float yaw = 0.0f;

        Transform transform;

        private void OnCreate()
        {
            Session.InputMap.AddMouseMove(OnMouseMove);

            transform = Entity.GetComponent<Transform>();
        }

        private void OnMouseMove(object sender, MouseMoveEventArgs e)
        {
            // when cursor is hidden, move camera around
            if(Cursor.Mode == CursorMode.Disabled)
            {
                pitch -= e.DeltaPosition.Y;
                yaw += e.DeltaPosition.X;
            }
        }

        private void OnUpdate()
        {
            if (Cursor.Mode == CursorMode.Disabled)
            {
                // clamp pitch so camera cannot go upside down/backwards
                pitch = Math.Min(Math.Max(pitch, -89.99f), 89.99f);

                

                // set new rotation to the values
                transform.LocalRotation = Quaternion.FromEuler(pitch * Math.Deg2Rad, yaw * Math.Deg2Rad, 0.0f);
            }
        }
    }
}
