using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Material : Asset
    {
        public MaterialTemplate Template
        {
            get => Runtime.Material_GetTemplate(ID) as MaterialTemplate;
        }

        internal Material(UUID id)
            : base(id)
        { }

        public Material(string path)
            : base(Runtime.Material_Load(path))
        { }

        /// <summary>
        /// Sets the value of the Material Shader input with the given name.
        /// </summary>
        /// <param name="name">The name of the Shader input.</param>
        /// <param name="value">The value of the Shader input.</param>
        public void Set<T>(string name, T t)
        {
            System.Type type = t.GetType();
            if(type.IsArray)
            {
                Debug.Error("Cannot use Material.Set() for an array. Try Material.SetArray().");
                return;
            } else if (t is Asset asset)
            {
                Runtime.Material_Set(ID, name, (int)Type.Object, asset.ID.ToString());
            } else if(type.IsValueType)
            {
                Runtime.Material_Set(ID, name, (int)typeof(T).ToMintyType(), t.ToString());
            }
            else
            {
                Debug.Error($"Material.Set({name}, {t}) failed. Invalid Type ({type.FullName}).");
            }
        }

        public void SetArray<T>(string name, T[] ts)
        {
            if(ts == null)
            {
                Debug.Error("Cannot set to null within Material.SetArray(). Try an empty array.");
                return;
            }

            System.Type type = typeof(T);
            if (type.IsSubclassOf(typeof(Asset)))
            {
                UUID[] ids = ts.Select(t => (t as Asset).ID).ToArray();
                string idsString = string.Join(",", ids);
                Runtime.Material_Set(ID, name, (int)Type.Object, idsString);
            }
            else if (type.IsValueType)
            {
                string valuesString = string.Join(",", ts);
                Runtime.Material_Set(ID, name, (int)typeof(T).ToMintyType(), valuesString);
            }
            else
            {
                string valuesString = string.Join(",", ts);
                Debug.Error($"Material.SetArray({name}, {valuesString}) failed. Invalid Type ({type.FullName}).");
            }
        }
    }
}
