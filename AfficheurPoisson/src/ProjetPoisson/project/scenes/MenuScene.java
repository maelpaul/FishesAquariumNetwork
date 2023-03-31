package ProjetPoisson.project.scenes;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.inputs.KeyboardManager;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.BasicBindableObject;
import ProjetPoisson.mightylib.resources.texture.Icon;
import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.mightylib.resources.texture.TextureParameters;
import ProjetPoisson.mightylib.scene.Scene;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.command.CommandAnalyser;
import ProjetPoisson.project.command.Terminal;
import org.joml.Vector2f;
import org.joml.Vector2i;
import org.joml.Vector3f;
import org.lwjgl.glfw.GLFW;


import java.io.File;
import java.util.ArrayList;
import java.util.Objects;

public class MenuScene extends Scene {
    private Text text;

    private RectangleRenderer renderer;

    private Terminal terminal;

    private CommandAnalyser analyser;

    public void init(String[] args) {
        super.init(args, new BasicBindableObject().setQualityTexture(TextureParameters.REALISTIC_PARAMETERS));

        if (Resources.getInstance().isExistingResource(Icon.class, "Kraken"))
            mainContext.getWindow().setIcon(Resources.getInstance().getResource(Icon.class, "Kraken"));

        /// SCENE INFORMATION ///

        main3DCamera.setPos(new Vector3f(0, 0, 0));
        setClearColor(52, 189, 235, 1f);

        /// RENDERERS ///

        Vector2i windowSize = mainContext.getWindow().getInfo().getSizeCopy();

        renderer = new RectangleRenderer("texture2D");
        renderer.switchToTextureMode("background");
        renderer.setSizePix(windowSize.x, windowSize.y);

        text = new Text();
        text.setText("Aquarium poisson")
                .setFont("bahnschrift")
                .setAlignment(ETextAlignment.Center)
                .setReference(EDirection.None)
                .setPosition(new Vector2f(windowSize.x * 0.5f, windowSize.y * 0.2f))
                .setColor(new Color4f(1, 1, 1, 1))
                .setFontSize(40);

        terminal = new Terminal(new Vector2f(0,windowSize.y), new Vector2f(windowSize.y * 0.5f,windowSize.y * 0.5f));
        Configuration conf = Resources.getInstance().getResource(Configuration.class, "affichage");

        analyser = new CommandAnalyser();

        Configuration configuration = Resources.getInstance().getResource(Configuration.class, "affichage");

        ArrayList<String> fishesFileName = new ArrayList<>();
        File folderFish = new File(configuration.getPathForFishesResources());
        for (String childPath : Objects.requireNonNull(folderFish.list())) {
            String fileName = childPath.substring(childPath.lastIndexOf("/") + 1, childPath.lastIndexOf("."));
            Resources.getInstance().createAndInit(Texture.class, fileName, configuration.getPathForFishesResources() + "/" + childPath);
            fishesFileName.add(fileName);
        }
    }

    public void update() {
        super.update();

        KeyboardManager manager = mainContext.getKeyboardManager();

        for (char letter = 'a'; letter <= 'z'; ++letter){
            if (manager.keyPressed(GLFW.GLFW_KEY_A + (letter - 'A'))){
                System.out.println(letter);
            }
        }

        terminal.update(mainContext.getInputManager(), mainContext.getSystemInfo());

        if (terminal.shouldProcessCommand()){
            String result = analyser.analyseCommand(terminal.getCommandText());

            if (result != null) {
                if (result.equals("¤¤clear¤¤"))
                    terminal.clearResultText();
                else
                    terminal.addToResultText(result);

                terminal.saveCommand()
                        .clearCommandText();
            }
        }
    }


    public void display() {
        super.setVirtualScene();
        clear();

        renderer.display();

        text.display();

        terminal.display();

        super.setAndDisplayRealScene();
    }


    public void unload() {
        super.unload();

        renderer.unload();

        text.unload();
        terminal.unload();
    }
}
