package ProjetPoisson.project.scenes;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.graphics.shader.ShaderManager;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningBehaviour;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningOption;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningType;
import ProjetPoisson.mightylib.physics.tweenings.type.FloatTweening;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.BasicBindableObject;
import ProjetPoisson.mightylib.resources.texture.Icon;
import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.mightylib.resources.texture.TextureParameters;
import ProjetPoisson.mightylib.scene.Scene;
import ProjetPoisson.mightylib.util.Timer;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.Message;
import ProjetPoisson.project.command.CommandAnalyser;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.command.Terminal;
import ProjetPoisson.project.display.FishManager;
import ProjetPoisson.project.threads.ClientThread;
import org.joml.Vector2f;
import org.joml.Vector2i;
import org.joml.Vector3f;

public class MenuScene extends Scene {
    public enum EConnectionState {
        Disconnected,
        FirstMessageSent,
        StartingConnectionProcedure,
        ReceiveIdConnection,
        CreatedAllFished,
        Connected
    }

    public class ConnectionStateContainer {
        private EConnectionState state = EConnectionState.Disconnected;

        public EConnectionState get() { return state; }

        public void set(EConnectionState state) { this.state = state; }
    }

    public final static float PING_TIME = 35; // 40 in reality
    public final static float ATTEMPT_CONNECTION_TIME = 2;

    public final static float WAIT_FOR_EXIT = 5;

    public final static float UPDATE_FISH_REQUEST_TIME = 1;

    private Timer pingTimer;
    private Timer attemptConnexionTimer;

    private Timer updateFishRequestTime;

    private Text text;

    private RectangleRenderer renderer;

    private Terminal terminal;

    private CommandAnalyser analyser;

    private FishManager fishManager;
    private Texture displacementMap;
    private FloatTweening displacementMapTweening;

    private ClientThread client;

    private ConnectionStateContainer currentState;
    private RectangleRenderer connectionStatusIcon;
    private Text connectionStatusMessage;
    private boolean firstDisconnect = false;

    int waitResponse;

    private String idClient = "??";

    public void init(String[] args) {
        super.init(args, new BasicBindableObject().setQualityTexture(TextureParameters.REALISTIC_PARAMETERS));

        pingTimer = new Timer();
        pingTimer.start(PING_TIME);
        pingTimer.stop();

        attemptConnexionTimer = new Timer();
        attemptConnexionTimer.start(ATTEMPT_CONNECTION_TIME);

        updateFishRequestTime = new Timer();
        updateFishRequestTime.start(UPDATE_FISH_REQUEST_TIME);

        waitResponse = -1;
        if (Resources.getInstance().isExistingResource(Icon.class, "Kraken"))
            mainContext.getWindow().setIcon(Resources.getInstance().getResource(Icon.class, "Kraken"));

        /// SCENE INFORMATION ///
        client = new ClientThread();
        client.start();

        Vector2i windowSize = mainContext.getWindow().getInfo().getSizeCopy();
        terminal = new Terminal(new Vector2f(0,windowSize.y), new Vector2f(windowSize.x * 0.5f,windowSize.y * 0.5f));

        main3DCamera.setPos(new Vector3f(0, 0, 0));
        setClearColor(52, 189, 235, 1f);

        /// RENDERERS ///

        renderer = new RectangleRenderer("texture2DDisplacement");
        renderer.switchToTextureMode("background");
        renderer.setSizePix(windowSize.x, windowSize.y);

        float reductionRatio = 0.5f;
        connectionStatusIcon = new RectangleRenderer("texture2D");
        connectionStatusIcon.switchToTextureMode("disconnectedIcon");
        connectionStatusIcon.setSizeToTexture();
        connectionStatusIcon.setScale(
                new Vector3f(connectionStatusIcon.scale().x * reductionRatio, connectionStatusIcon.scale().y * reductionRatio, connectionStatusIcon.scale().z)
        );
        connectionStatusIcon.setPosition(new Vector2f(windowSize.x * 0.02f, windowSize.y * 0.025f));

        text = new Text();
        text.setText("Aquarium poisson")
                .setFont("bahnschrift")
                .setAlignment(ETextAlignment.Center)
                .setReference(EDirection.None)
                .setPosition(new Vector2f(windowSize.x * 0.5f, windowSize.y * 0.2f))
                .setColor(new Color4f(1, 1, 1, 1))
                .setFontSize(40);

        connectionStatusMessage = new Text();
        text.copyTo(connectionStatusMessage);

        connectionStatusMessage.setText("disconnected")
                .setFontSize(30)
                .setReference(EDirection.Left)
                .setColor(new Color4f())
                .setPosition(new Vector2f(connectionStatusIcon.position().x + connectionStatusIcon.scale().x + 10,
                        connectionStatusIcon.position().y + connectionStatusIcon.scale().x * 0.5f));

        Configuration configuration = Resources.getInstance().getResource(Configuration.class, "affichage");

        fishManager = new FishManager(mainContext.getWindow().getInfo(), configuration);

        displacementMap = Resources.getInstance().getResource(Texture.class, "displacementMap");
        ShaderManager.getInstance().getShader(renderer.getShape().getShaderId()).glUniform("displacementMap", 1);

        displacementMapTweening = new FloatTweening();
        displacementMapTweening.setTweeningValues(ETweeningType.Linear, ETweeningBehaviour.InOut)
                .setTweeningOption(ETweeningOption.LoopReversed)
                .initTwoValue(15f, 0f, 15f);

        currentState = new ConnectionStateContainer();
        analyser = new CommandAnalyser(currentState, fishManager);

        setConnectionState(EConnectionState.Disconnected);
    }

    public void setConnectionState(EConnectionState connectionState){
        currentState.set(connectionState);

        if (connectionState == EConnectionState.Disconnected){
            connectionStatusIcon.switchToTextureMode("disconnectedIcon");
            connectionStatusMessage.setColor(new Color4f(140f / 255f, 8f / 255f, 8f / 255f, 1));
            connectionStatusMessage.setText("Disconnected");
        } else if (connectionState == EConnectionState.Connected){
            connectionStatusIcon.switchToTextureMode("connectedIcon");
            connectionStatusMessage.setColor(new Color4f(43f / 255f, 228f / 255f, 5f / 255f, 1));
            connectionStatusMessage.setText("Connected as " + idClient);
        } else {
            connectionStatusIcon.switchToTextureMode("disconnectedIcon");
            connectionStatusMessage.setColor(new Color4f(187f / 255f, 187f / 255f, 65f / 255f, 1));
            connectionStatusMessage.setText("Connecting as " + idClient + " ...");
        }
    }


    public void initializeConnection() {
        switch (currentState.get()){
            case Disconnected:
                if (client.didReceiveMessage()){
                    Message[] messages = client.message();
                    if (messages.length == 1) {
                        terminal.addToResultText("<" + messages[0].getMessage());
                        client.sendMessage("hello\n");
                        setConnectionState(EConnectionState.StartingConnectionProcedure);
                    }
                }

                break;
            case StartingConnectionProcedure:
                if (client.didReceiveMessage()){
                    Message[] messages = client.message();
                    if (messages.length == 1) {
                        terminal.addToResultText("<" + messages[0].getMessage());
                        String[] parts = messages[0].getMessage().split(" ");
                        idClient = parts[1];
                        if (parts.length == 3)
                            idClient = parts[2];
                        client.sendMessage("getFishes\n");
                        setConnectionState(EConnectionState.FirstMessageSent);
                    }
                }
                break;
            case FirstMessageSent:
                if (client.didReceiveMessage()){
                    Message[] messages = client.message();
                    if (messages.length == 1) {
                        analyseGetFished(messages[0]);
                        //fishMessage = ">list [PoissonRouge at 92x40,10x4,5] [PoissonClown at 22x80,12x6,5]\n";

                        setConnectionState(EConnectionState.Connected);
                    }
                }
                break;
        }
    }

    public void analyseGetFished(Message message){
        String fishMessage = message.getMessage();
        //fishMessage = ">list [PoissonRouge at 92x40,10x4,5] [PoissonClown at 22x80,12x6,5]\n";

        //terminal.addToResultText(message.getMessage().replace(">", "<"));

        if (fishMessage.contains("]")){
            String[] fishesArguments = fishMessage
                    .replace("list", "")
                    .replace("\n", "")
                    .replace("[", "")
                    .split("]");

            for (String fishArgument : fishesArguments){
                fishManager.processFishUpdate(fishArgument.trim());
            }
        }
    }

    public void update() {
        super.update();
        fishManager.update();

        if (firstDisconnect==false) {
            if (client.isDisconnected()) {
                terminal.addToResultText("Client Disconnected");
                setConnectionState(EConnectionState.Disconnected);
                this.firstDisconnect = true;
            }
            if (client.getClient().isConnected()) {
                if (currentState.get() == EConnectionState.Connected) {
                    updateFishRequestTime.update();
                    if (updateFishRequestTime.isFinished()) {
                        if (waitResponse == -1) {
                            //waitResponse = client.sendMessage("getFishes");
                        } else if (client.didReceiveMessage()) {
                            for (Message message : client.message()) {
                                if (message.getId() == waitResponse) {
                                    analyseGetFished(message);

                                    waitResponse = -1;
                                }
                            }
                        }
                        updateFishRequestTime.resetStart();
                    }
                } else {
                    initializeConnection();
                    attemptConnexionTimer.stop();
                }
            } else {
                attemptConnexionTimer.update();

                if (!client.getClient().isTryingConnection() && attemptConnexionTimer.isFinished()) {
                    client.shouldTryConnection();
                    attemptConnexionTimer.resetStart();
                }
            }
        }

        terminal.update(mainContext.getInputManager(), mainContext.getSystemInfo());

        if (terminal.shouldProcessCommand()){
            String commandText = terminal.getCommandText();
            ResultCommand result = analyser.analyseCommand(commandText);

            if (result != null) {
                if (result.getResultAction() == ResultCommand.EResultAction.Clear)
                     terminal.clearResultText();
                else if (result.getResultAction() == ResultCommand.EResultAction.Quit)
                    this.sceneManagerInterface.exit(0);
                else
                    terminal.addToResultText(result.getPromptResult());

                terminal.saveCommand()
                        .clearCommandText();

                if (EConnectionState.Connected == currentState.get() && result.getResultAction() == ResultCommand.EResultAction.SendServer)
                    client.sendMessage(commandText.replace("/", ""));
            }
        }

        displacementMapTweening.update();
        pingTimer.update();
        if (pingTimer.isFinished()){
            // DO THINGS
            pingTimer.resetStart();
        }

        ShaderManager.getInstance().getShader(renderer.getShape().getShaderId()).glUniform("time", displacementMapTweening.value());
    }


    public void display() {
        super.setVirtualScene();
        clear();

        displacementMap.bind(1);
        renderer.display();

        fishManager.display();

        text.display();

        terminal.display();

        connectionStatusIcon.display();
        connectionStatusMessage.display();

        super.setAndDisplayRealScene();
    }


    public void unload() {
        super.unload();

        renderer.unload();

        text.unload();
        terminal.unload();

        client.interrupt();

        connectionStatusIcon.unload();
        connectionStatusMessage.unload();

        displacementMap.unload();

        fishManager.unload();
    }
}
