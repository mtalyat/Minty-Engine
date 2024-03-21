using MintyEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class Link : Script
{
    private void OnLoad()
    {
        Input.SetInputMap(Session.InputMap);
    }
}