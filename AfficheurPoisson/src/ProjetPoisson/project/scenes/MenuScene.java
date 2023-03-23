package ProjetPoisson.project.scenes;

import ProjetPoisson.mightylib.graphics.GUI.BackgroundlessButton;
import ProjetPoisson.mightylib.graphics.GUI.GUIList;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.inputs.InputManager;
import ProjetPoisson.mightylib.inputs.KeyboardManager;
import ProjetPoisson.mightylib.resources.texture.TextureParameters;
import ProjetPoisson.mightylib.scene.Scene;
import ProjetPoisson.mightylib.sounds.SoundManager;
import ProjetPoisson.mightylib.sounds.SoundSource;
import ProjetPoisson.mightylib.sounds.SoundSourceCreationInfo;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import ProjetPoisson.mightylib.util.math.MightyMath;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningBehaviour;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningOption;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningType;
import ProjetPoisson.mightylib.physics.tweenings.type.FloatTweening;
import ProjetPoisson.project.command.Terminal;
import ProjetPoisson.project.lib.ActionId;
import org.joml.Vector2f;
import org.joml.Vector2i;
import org.joml.Vector3f;
import org.lwjgl.glfw.GLFW;

public class MenuScene extends Scene {
    private GUIList guiList;
    private BackgroundlessButton buttonQuit;

    private Text text;

    private Terminal terminal;

    public void init(String[] args) {
        super.init(args, TextureParameters.REALISTIC_PARAMETERS);
        /// SCENE INFORMATION ///

        main3DCamera.setPos(new Vector3f(0, 0, 0));
        setClearColor(52, 189, 235, 1f);

        /// RENDERERS ///

        Vector2i windowSize = mainContext.getWindow().getInfo().getSizeCopy();

       /*BackgroundlessButton buttonMainScene = new BackgroundlessButton(mainContext);
        buttonMainScene.Text.setFont("arial")
                .setAlignment(ETextAlignment.Center)
                .setReference(EDirection.None)
                .setPosition(new Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f))
                .setFontSize(40)
                .setText("Main menu");

        buttonMainScene.Text.copyTo(buttonMainScene.OverlapsText);
        buttonMainScene.OverlapsText.setColor(new Color4f(0.3f))
                .setText("->Main menu<-");

        buttonQuit = buttonMainScene.copy();
        buttonQuit.Text.setPosition(new Vector2f(windowSize.x * 0.5f, windowSize.y * 0.9f))
                .setText("Quit");

        buttonQuit.Text.copyTo(buttonQuit.OverlapsText);
        buttonQuit.OverlapsText.setColor(new Color4f(0.3f))
                .setText("->Quit<-");

        guiList = new GUIList(mainContext.getInputManager(), mainContext.getMouseManager());
        guiList.setupActionInputValues(ActionId.SELECT_UP, ActionId.SELECT_DOWN);
        guiList.GUIs.put(0, buttonMainScene);
        guiList.GUIs.put(1, buttonQuit);
        guiList.ShouldLoop = false;*/

        text = new Text();
        text.setText("Aquarium poisson")
                .setFont("bahnschrift")
                .setAlignment(ETextAlignment.Center)
                .setReference(EDirection.None)
                .setPosition(new Vector2f(windowSize.x * 0.5f, windowSize.y * 0.2f))
                .setFontSize(40);

        terminal = new Terminal(new Vector2f(0,windowSize.y), new Vector2f(windowSize.y * 0.5f,windowSize.y * 0.5f));
        terminal.resultText.setText("Write command:");
    }

    public void update() {
        super.update();

        /*guiList.update();

        if (mainContext.getInputManager().input(ActionId.ENTER) || mainContext.getInputManager().inputPressed(ActionId.LEFT_CLICK)){
            Integer id = guiList.getSelected();
            if (id != null) {
                switch (id) {
                    case 0:
                        sceneManagerInterface.setNewScene(new MainScene(), new String[]{""});
                        break;
                    case 1:
                    default:
                        sceneManagerInterface.exit(0);
                        break;
                }
            }
        }*/

        KeyboardManager manager = mainContext.getKeyboardManager();

        for (char letter = 'a'; letter <= 'z'; ++letter){
            if (manager.keyPressed(GLFW.GLFW_KEY_A + (letter - 'A'))){
                System.out.println(letter);
            }
        }

        terminal.update(mainContext.getKeyboardManager(), mainContext.getSystemInfo());
    }


    public void display() {
        super.setVirtualScene();
        clear();

        //guiList.display();

        text.display();

        terminal.display();

        super.setAndDisplayRealScene();
    }


    public void unload() {
        super.unload();

        //guiList.unload();

        text.unload();
        terminal.unload();
    }
}
