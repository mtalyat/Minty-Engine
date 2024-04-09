using MintyEngine;

public class TestScript : Script
{
    void OnCreate()
    {

    }

    void OnLoad()
    {

    }

    void OnEnable()
    {

    }

    void OnUpdate()
    {

    }

    void OnDisable()
    {

    }

    void OnUnload()
    {

    }

    void OnDestroy()
    {

    }

    void OnPointerEnter()
    {
        Debug.Log($"Enter {Entity.Name}");
    }

    void OnPointerHover()
    {

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