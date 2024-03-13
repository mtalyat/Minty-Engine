using MintyEngine;

namespace TestProject
{
    class TestScript : Script
    {
        public float time;

        private Transform transform;

        void OnCreate()
        {
            Debug.Log("TestClass.OnCreate()");
            Debug.Log($"Entity is {Entity}");
        }

        void OnLoad()
        {
            Debug.Log("TestClass.OnLoad()");

            transform = Entity.GetComponent<Transform>();
        }

        void OnEnable()
        {
            Debug.Log("TestClass.OnEnable()");
        }

        void OnUpdate()
        {
            Debug.Log($"Time: {time:0.00}s");
            time -= Time.ElapsedTime;

            if (time <= 0.0f)
            {
                Destroy(Entity);
            }

            //Vector3 pos = transform.LocalPosition;
            //pos.Z += Time.ElapsedTime;
            //transform.LocalPosition = pos;

            //transform.LocalScale /= 1.0f + Time.ElapsedTime * 0.1f;

            //Debug.Log(transform.LocalPosition);
        }

        void OnDisable()
        {
            Debug.Log("TestClass.OnDisable()");
        }

        void OnUnload()
        {
            Debug.Log("TestClass.OnUnload()");
        }

        void OnDestroy()
        {
            Debug.Log("TestClass.OnDestroy()");
        }
    }
}