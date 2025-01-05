using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public static class SceneManager
    {
        /// <summary>
        /// Loads the given Scene. Unloads the current Scene first.
        /// </summary>
        /// <param name="scene"></param>
        public static void Load(string path)
        {
            Runtime.SceneManager_Load(path);
        }
    }
}
