using MintyEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestProject
{
    public class PlayerController : Script
    {
        private float moveSpeed = 1.0f;

        private Vector3 input;

        private Transform transform;

        private void OnCreate()
        {
            // add actions to input map
            Session.InputMap.AddKeysDown(OnKeyMove, Key.D, Key.A, Key.W, Key.S, Key.LeftShift, Key.Space);
            Session.InputMap.AddKeysUp(OnKeyMove, Key.D, Key.A, Key.W, Key.S, Key.LeftShift, Key.Space);
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
    }
}
