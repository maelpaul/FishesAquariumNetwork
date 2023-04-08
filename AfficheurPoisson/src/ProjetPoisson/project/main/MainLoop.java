package ProjetPoisson.project.main;

import ProjetPoisson.mightylib.graphics.shader.ShaderManager;
import ProjetPoisson.mightylib.inputs.keyboardlanguage.AZERTYKeyboardLanguage;
import ProjetPoisson.mightylib.main.*;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.Icon;
import ProjetPoisson.mightylib.scene.SceneManager;
import ProjetPoisson.mightylib.sounds.SoundManager;
import ProjetPoisson.project.scenes.MenuScene;
import org.joml.Vector2i;
import org.lwjgl.Version;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11C.*;
import static org.lwjgl.opengl.GL20.GL_SHADING_LANGUAGE_VERSION;

public final class MainLoop {
    private final ContextManager contextManager;
    private final SceneManager sceneManager;

    private final static float NANO_IN_SECOND = 1000000000.0f;
    private final static float TPS = 60.0f;

    private final static float FPS = 60.0f;

    private final static double TICK_TIME = NANO_IN_SECOND / TPS;
    private final static double FRAME_TIME = NANO_IN_SECOND / FPS;

    private final static String projectName = "Afficheur poissons";

    MainLoop(){
        System.out.println("--Start program. ");
        System.out.println("--Load libraries.");

        if (loadLibraries() == -1){
            exit(ListError.LIBRARIES_LOAD_FAIL);
        }

        System.out.println("--Create main context.");
        contextManager = ContextManager.getInstance();

        WindowCreationInfo wci = new WindowCreationInfo();
        wci.Size = new Vector2i(1280, 720);
        wci.VirtualSize = new Vector2i(1280, 720);
        wci.WindowName = projectName;
        wci.Fullscreen = false;

        contextManager.createDefaultContext(wci);

        Context context = contextManager.getContext("Main");
        Window window = context.getWindow();

        if (!window.getInfo().isWindowCreated()){
            exit(ListError.WINDOW_CREATION_FAIL);
        }

        System.out.println("--Create ShaderManager");
        ShaderManager shaderManager = ShaderManager.getInstance();
        //shaderManager.forceShaderVersion(140);
        System.out.println("--Create Resources");
        Resources resource = Resources.getInstance();

        System.out.println("--Create SceneManager");
        sceneManager = new SceneManager(this);

        ProjectLoading.init();
        ProjectLoading.ContextLoading(context);

        resource.init();
        sceneManager.init(new MenuScene(), new String[]{""});

        System.out.println("\n" + Version.getVersion());
        System.out.println(glfwGetVersionString());
        System.out.println("GL VENDOR   : " + glGetString(GL_VENDOR));
        System.out.println("GL RENDERER : " + glGetString(GL_RENDERER));
        System.out.println("GL VERSION  : " + glGetString(GL_VERSION));
        System.out.println("GLSL VERSION :" + glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    void run(){
        // Set loop parameters
        int ticks = 0;
        int frames = 0;

        double lastTick = 0.0;
        double lastFrame = 0.0;
        double lastSecond = 0.0;

        long start = System.nanoTime();

        Context mainContext = contextManager.getMainContext();
        mainContext.getKeyboardManager().setKeyboardLanguage(AZERTYKeyboardLanguage.getInstance());

        Window window = mainContext.getWindow();

        while (!window.wantExit()){
            if (System.nanoTime() - start - lastTick >= TICK_TIME) {
                GameTime.update();
                sceneManager.update();
                sceneManager.dispose();
                ++ticks;
                lastTick += TICK_TIME;
            } else if (System.nanoTime() - start - lastFrame >= FRAME_TIME) {
                sceneManager.display();
                window.dispose();
                ++frames;
                lastFrame += FRAME_TIME;
            }

            if (System.nanoTime() - start - lastSecond >= NANO_IN_SECOND) {
                if (Main.admin) window.setTitle(projectName + "| FPS:" + frames + "; TPS:" + ticks);

                ticks = frames = 0;
                lastSecond += NANO_IN_SECOND;
            }
        }

        exit(ListError.NO_ERROR);
    }


    public void exit(int status){
        if (status != ListError.LIBRARIES_LOAD_FAIL){
            // Terminate GLFW and free the error callback
            preUnload();
        }

        if (sceneManager != null)
            sceneManager.unload();

        if (contextManager != null)
            contextManager.unload();

        afterUnload();

        if (status != ListError.NO_ERROR){
            System.err.println("Exit with error "  + status);
            System.exit(status);
        } else {
            System.out.println("Exit without error");
            System.exit(0);
        }
    }


    private int loadLibraries() {
        // Initialize GLFW. Most GLFW functions will not work before doing this.
        if (!glfwInit()) {
            System.err.println("GLFW fail to initialize");
            return -1;
        }

        SoundManager soundManager = SoundManager.getInstance();
        if (!soundManager.init()){
            System.err.println("SoundManager fail to initialize");
            return -1;
        }

        return 0;
    }


    private void preUnload(){
        SoundManager.getInstance().unloadSoundSource();
    }

    private void afterUnload(){
        glfwTerminate();
        SoundManager.getInstance().unload();
    }
}
