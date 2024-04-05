using MintyEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class PlayerController : Script
{
    private float moveSpeed = 1.0f;

    private Vector3 input;

    private Transform transform;

    private void OnCreate()
    {
        // add actions to input map
        foreach (Key key in new Key[] { Key.D, Key.A, Key.W, Key.S, Key.LeftShift, Key.Space })
        {
            Session.InputMap.OnKeyDown[key] += OnKeyMove;
            Session.InputMap.OnKeyUp[key] += OnKeyMove;
        }

        Session.InputMap.OnGamepadConnect += OnGamepadConnect;
        Session.InputMap.OnGamepadDisconnect += OnGamepadDisconnect;

        foreach(GamepadButton button in new GamepadButton[] { GamepadButton.ButtonSouth, GamepadButton.ButtonNorth, GamepadButton.ButtonEast, GamepadButton.ButtonWest })
        {
            Session.InputMap.OnGamepadDown[button] += OnGamepadDown;
            Session.InputMap.OnGamepadUp[button] += OnGamepadUp;
        }

        foreach(GamepadAxis axis in new GamepadAxis[] { GamepadAxis.LeftX, GamepadAxis.LeftY })
        {
            Session.InputMap.OnGamepadChange[axis] += OnGamepadAxis;
        }
    }

    private void OnLoad()
    {
        transform = Entity.GetComponent<Transform>();
    }

    private void OnUpdate()
    {
        float movement = moveSpeed * Time.ElapsedTime;

        if (Input.IsKeyDown(Key.LeftControl))
        {
            movement *= 10.0f;
        }

        Vector3 move = movement * input.X * transform.Right +
            movement * input.Y * transform.Up +
            movement * input.Z * transform.Forward;

        transform.LocalPosition += move;
    }

    private void OnKeyMove(object sender, KeyPressEventArgs e)
    {
        float direction = e.Action == ButtonAction.Down ? 1.0f : -1.0f;

        switch (e.Key)
        {
            case Key.D:
                input.X += direction;
                break;
            case Key.A:
                input.X -= direction;
                break;
            case Key.LeftShift:
                input.Y += direction;
                break;
            case Key.Space:
                input.Y -= direction;
                break;
            case Key.W:
                input.Z += direction;
                break;
            case Key.S:
                input.Z -= direction;
                break;
        }
    }

    private void OnGamepadConnect(object sender, GamepadConnectionEventArgs e)
    {
        Debug.Log($"Gamepad connect: {e.Controller}");
    }

    private void OnGamepadDisconnect(object sender, GamepadConnectionEventArgs e)
    {
        Debug.Log($"Gamepad disconnect: {e.Controller}");
    }

    private void OnGamepadDown(object sender, GamepadButtonEventArgs e)
    {
        Debug.Log($"Down: {e.Button}");
    }

    private void OnGamepadUp(object sender, GamepadButtonEventArgs e)
    {
        Debug.Log($"Up: {e.Button}");
    }

    private void OnGamepadAxis(object sender, GamepadAxisEventArgs e)
    {
        Debug.Log($"Axis: {e.Axis}");
    }
}