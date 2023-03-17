package ProjetPoisson.project.scenes;

import ProjetPoisson.mightylib.graphics.game.FullTileMapRenderer;
import ProjetPoisson.mightylib.main.GameTime;
import ProjetPoisson.mightylib.physics.tweenings.type.FloatTweening;
import ProjetPoisson.mightylib.resources.map.TileMap;
import ProjetPoisson.mightylib.graphics.renderer._2D.Animation2DRenderer;;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.animation.AnimationData;
import ProjetPoisson.mightylib.resources.animation.Animator;
import ProjetPoisson.mightylib.inputs.InputManager;
import ProjetPoisson.mightylib.scene.Camera2D;
import ProjetPoisson.mightylib.scene.Scene;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningBehaviour;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningOption;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningType;
import ProjetPoisson.mightylib.physics.tweenings.type.Vector2fTweening;
import ProjetPoisson.mightylib.util.math.MightyMath;
import ProjetPoisson.project.lib.ActionId;
import org.joml.Vector2f;
import org.joml.Vector2i;
import org.joml.Vector3f;

public class MainScene extends Scene {
    public void init(String[] args) {
        super.init(args);
        /// SCENE INFORMATION ///

        main3DCamera.setPos(new Vector3f(0, 0, 0));
        //setClearColor(52, 189, 235, 1f);
        setClearColor(0, 0, 0, 1f);

        /// RENDERERS ///


    }


    public void update() {
        super.update();

        InputManager inputManager = mainContext.getInputManager();

    }


    public void display() {
        super.setVirtualScene();
        clear();



        super.setAndDisplayRealScene();
    }


    public void unload() {
        super.unload();

    }
}
