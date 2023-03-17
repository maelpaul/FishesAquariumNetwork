package ProjetPoisson.mightylib.scene;

import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.mightylib.inputs.InputManager;
import ProjetPoisson.mightylib.main.Context;
import ProjetPoisson.mightylib.main.ContextManager;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.sounds.SoundManager;
import ProjetPoisson.project.main.Main;
import ProjetPoisson.project.main.MainLoop;
import ProjetPoisson.mightylib.graphics.shader.ShaderManager;
import ProjetPoisson.mightylib.util.commands.Commands;

public class SceneManager {
    private final SceneManagerInterface sceneInterface;
    private Scene currentScene;

    private final MainLoop loop;

    private final Commands commands;
    private final SoundManager soundManager;

    public SceneManager(MainLoop mLoop){
        this.loop = mLoop;

        sceneInterface = new SceneManagerInterface();

        commands  = new Commands();
        soundManager = SoundManager.getInstance();
    }


    public void init(Scene firstScene, String[] firstArguments){
        System.out.println("--Init SceneManager");
        ShaderManager.getInstance().load();
        Resources.getInstance().load();

        sceneInterface.setNewScene(firstScene, firstArguments);

        changeScene();
    }


    public void update(){
        // Command
        if (Main.admin){
            updateMain();

        }

        if(sceneInterface.isWantingChange())
            changeScene();

        if (sceneInterface.WantQuit)
            exit(sceneInterface.ExitStatus);

        currentScene.update();

        soundManager.lateUpdate();
    }


    private void updateMain(){
        Context mainContext = ContextManager.getInstance().getMainContext();
        InputManager mainInputManager = mainContext.getInputManager();

        if (mainInputManager.inputPressed(InputManager.COMMAND) || commands.isWriteCommands)
            commands.writeCommand();

        if (mainInputManager.inputPressed(InputManager.RELOAD_TEXTURE))
            Resources.getInstance().reload(Texture.class);
    }


    public void display(){
        currentScene.display();
    }


    public void dispose(){
        ContextManager.getInstance().dispose();
        currentScene.dispose();
    }



    private void changeScene(){
        if (Main.admin)
            commands.removeSpecificCommand();

        if(currentScene != null){
            System.out.println("--Unload scene" + currentScene.getClass().getName());
            currentScene.unload();
        }

        SoundManager.getInstance().clearAwaitedSong();

        // Assign the new scene
        currentScene = sceneInterface.getNewScene();
        assert currentScene != null;
        currentScene.setSceneManagerInterface(sceneInterface);
        System.out.println("--Init scene" + currentScene.getClass().getName());
        currentScene.init(sceneInterface.getChangeArgs());

        sceneInterface.reset();
    }


    public void exit(int status){
        loop.exit(status);
    }


    public void unload(){
        System.out.println("--Unload SceneManager");

        if (currentScene != null){
            System.out.println("--Unload scene" + currentScene.getClass().getName());
            currentScene.unload();
        }

        ShaderManager.getInstance().unload();
        Resources.getInstance().unload();
    }
}
