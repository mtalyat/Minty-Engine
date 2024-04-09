using MintyEngine;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class CameraController : Script
{
    private float pitch = 0.0f;
    private float yaw = 0.0f;

    Transform transform;

    Camera camera;

    private void OnCreate()
    {
        Session.InputMap.OnMouseMove += OnMouseMove;
        Session.InputMap.OnMouseDown[MouseButton.Left] += OnMouseClick;
        Session.InputMap.OnKeyDown[Key.F] += OnFDown;
    }

    private void OnLoad()
    {
        transform = Entity.GetComponent<Transform>();
        camera = Entity.GetComponent<Camera>();

        camera.Color = Color.Random;
    }

    private void OnUpdate()
    {
        if (Cursor.Mode == CursorMode.Disabled)
        {
            // clamp pitch so camera cannot go upside down/backwards
            pitch = Math.Min(Math.Max(pitch, -89.99f), 89.99f);
            yaw %= 360.0f;

            // set new rotation to the values
            transform.LocalRotation = Quaternion.FromEuler(pitch * Math.Deg2Rad, yaw * Math.Deg2Rad, 0.0f);
        }
    }

    private void OnFDown(object sender, KeyPressEventArgs e)
    {
        camera.FOV += 1.0f;
    }

    private void OnMouseMove(object sender, MouseMoveEventArgs e)
    {
        // when cursor is hidden, move camera around
        if (Cursor.Mode == CursorMode.Disabled)
        {
            pitch -= e.DeltaPosition.Y;
            yaw += e.DeltaPosition.X;
        }
    }

    private void OnMouseClick(object sender, MouseClickEventArgs e)
    {
        // toggle mouse
        if (Cursor.Mode == CursorMode.Normal)
        {
            Cursor.Mode = CursorMode.Disabled;
        }
        else
        {
            Cursor.Mode = CursorMode.Normal;
        }
    }
}