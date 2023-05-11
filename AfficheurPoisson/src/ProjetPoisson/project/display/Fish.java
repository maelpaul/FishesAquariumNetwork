package ProjetPoisson.project.display;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.main.WindowInfo;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningBehaviour;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningOption;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningType;
import ProjetPoisson.mightylib.physics.tweenings.type.FloatTweening;
import ProjetPoisson.mightylib.physics.tweenings.type.Vector2fTweening;
import ProjetPoisson.mightylib.util.Timer;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import ProjetPoisson.mightylib.util.math.EFlip;
import org.joml.Vector2f;
import org.joml.Vector3f;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class Fish {

    public enum EFishBehaviour {
        Straight, Natural, Teleport
    }

    private static final ArrayList<String> list;
    static {
        list = new ArrayList<>();
        for (EFishBehaviour value : EFishBehaviour.values())
            list.add(value.name());
    }
    public static ArrayList<String> GetFishBehaviourList(){
        return list;
    }

    private final WindowInfo windowInfo;

    private final Vector2fTweening goalPosition;
    private final Timer timer;
    private final RectangleRenderer renderer;

    private final FloatTweening sizeTweening;

    private final FloatTweening swimMovement;
    private Vector2f swimVector;
    private final Random random;
    private final EFishBehaviour behaviour;

    private final Text name;
    public EFishBehaviour getBehaviour() { return behaviour; }

    public Fish(WindowInfo windowInfo, String name, String textureName, EFishBehaviour behaviour, Vector2f positionPercentage, Vector2f sizePercentage){
        this.windowInfo = windowInfo;
        this.name = new Text();

        this.behaviour = behaviour;

        random = new Random();

        renderer = new RectangleRenderer("texture2D");
        renderer.switchToTextureMode(textureName);
        renderer.setSizePix(windowInfo.getVirtualSizeCopy().x * sizePercentage.x,
                            windowInfo.getVirtualSizeCopy().y * sizePercentage.y)
                .setReference(EDirection.None);

        renderer.setPosition(new Vector2f(windowInfo.getVirtualSizeCopy().x * positionPercentage.x,
                        windowInfo.getVirtualSizeCopy().y * positionPercentage.y));

        goalPosition = new Vector2fTweening();
        timer = new Timer();


        swimMovement = new FloatTweening();
        swimMovement.setTweeningValues(ETweeningType.Sinusoidal, ETweeningBehaviour.InOut)
                .setTweeningOption(ETweeningOption.LoopMirrored);

        sizeTweening = new FloatTweening();
        sizeTweening.setTweeningValues(ETweeningType.Quadratic, ETweeningBehaviour.InOut)
                .setTweeningOption(ETweeningOption.LoopReversed);

        float diff = 0.90f + random.nextFloat() * 0.05f;

        sizeTweening.initTwoValue(0.4f + random.nextFloat() * 0.4f,
                renderer.scale().x * diff, renderer.scale().x * (1 / diff));
        sizeTweening.initRandomTweening();

        swimVector = new Vector2f(0, 0);

        this.name.setText(name)
                .setFont("bahnschrift")
                .setFontSize(15)
                .setAlignment(ETextAlignment.Center)
                .setReference(EDirection.Down)
                .setColor(new Color4f(0, 0,0 , 0));
    }


    public void setShowName(boolean value) {
        name.color.setA((value) ? 1: 0);
    }

    public void setPosition(Vector2f newPosition){
        renderer.setPosition(newPosition);

        name.setPosition(newPosition.add(new Vector2f(0, -renderer.scale().y * 0.5f)));
    }

    public void update(){
        if (!timer.isFinished() && timer.isStarted()){
            goalPosition.update();
            swimMovement.update();
            timer.update();

            if (goalPosition.finished() || behaviour == EFishBehaviour.Teleport) {
                setPosition(goalPosition.goalValue());
            } else if (behaviour == EFishBehaviour.Natural) {
                setPosition(goalPosition.value());
            } else {
                setPosition(goalPosition.value().add(new Vector2f(swimVector).mul(swimMovement.value())));
            }
        }

        sizeTweening.update();
        renderer.setScale(
                new Vector3f(sizeTweening.value(),
                             renderer.scale().y /*sizeTweening.value()*/,
                             renderer.scale().z)
        );
    }

    public boolean finishedTravel(){
        return timer.isFinished() || !timer.isStarted();
    }
    public void display(){
        renderer.display();

        name.display();
    }

    public void travelToNewPosition(Vector2f positionPercentage, float time){
        Vector2f position = new Vector2f(
                windowInfo.getVirtualSizeCopy().x * positionPercentage.x,
                windowInfo.getVirtualSizeCopy().y * positionPercentage.y);

        ETweeningType swimType;
        switch (behaviour){
            case Natural:
                swimType = ETweeningType.Quadratic;
                break;
            case Straight:
            default:
                swimType = ETweeningType.Linear;
        }

        goalPosition.initTwoValue(time, this.renderer.get2DPosition(), position)
               .setTweeningValues(swimType, ETweeningBehaviour.InOut)
               .setTweeningOption(ETweeningOption.Direct);

        timer.start(time);

        renderer.setTextureFlip((position.x < this.renderer.get2DPosition().x)? EFlip.None: EFlip.Vertical);

        float numberTime = 1;
        while(time / numberTime > 1)
            numberTime ++;

        swimMovement.initTwoValue(time / (numberTime * 2), 0.0f, 1f);

        swimVector = goalPosition.goalValue()
                .sub(this.renderer.get2DPosition())
                .normalize()
                .perpendicular()
                .mul(random.nextFloat() * 10 + 20);
    }

    public void unload(){
        this.name.unload();
        renderer.unload();
    }
}
