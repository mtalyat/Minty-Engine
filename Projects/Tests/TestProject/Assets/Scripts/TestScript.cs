using MintyEngine;

public class TestScript : Script
{
    public float time;

    private Transform transform;

    void OnCreate()
    {
        Debug.Log("TestScript.OnCreate()");
        Debug.Log($"Entity is {Entity}");
    }

    void OnLoad()
    {
        Debug.Log("TestScript.OnLoad()");

        transform = Entity.GetComponent<Transform>();
    }

    void OnEnable()
    {
        Debug.Log("TestScript.OnEnable()");
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
        Debug.Log("TestScript.OnDisable()");
    }

    void OnUnload()
    {
        Debug.Log("TestScript.OnUnload()");
    }

    void OnDestroy()
    {
        Debug.Log("TestScript.OnDestroy()");
    }
}