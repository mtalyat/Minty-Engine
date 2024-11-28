using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Contains all of the Event methods, for testing purposes.
    /// </summary>
    public class Test : Script
    {
        void OnCreate()
        {
            Debug.Log($"Create {Entity.Name}");
        }

        void OnLoad()
        {
            Debug.Log($"Load {Entity.Name}");
        }

        void OnEnable()
        {
            Debug.Log($"Enable {Entity.Name}");
        }

        void OnUpdate()
        {
            Debug.Log($"Update {Entity.Name}");
        }

        void OnDisable()
        {
            Debug.Log($"Disable {Entity.Name}");
        }

        void OnUnload()
        {
            Debug.Log($"Unload {Entity.Name}");
        }

        void OnDestroy()
        {
            Debug.Log($"Destroy {Entity.Name}");
        }

        void OnPointerEnter()
        {
            Debug.Log($"Enter {Entity.Name}");
        }

        void OnPointerHover()
        {
            Debug.Log($"Hover {Entity.Name}");
        }

        void OnPointerExit()
        {
            Debug.Log($"Exit {Entity.Name}");
        }

        void OnPointerDown()
        {
            Debug.Log($"Down {Entity.Name}");
        }

        void OnPointerUp()
        {
            Debug.Log($"Up {Entity.Name}");
        }

        void OnPointerClick()
        {
            Debug.Log($"Click {Entity.Name}");
        }

        void OnPointerMove()
        {
            Debug.Log($"Move {Entity.Name}");
        }
    }
}
