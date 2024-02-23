using MintyEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestProject
{
    internal class Link : Script
    {
        private void OnLoad()
        {
            Input.SetInputMap(Session.InputMap);
        }
    }
}
