package ProjetPoisson.project.display;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.main.WindowInfo;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningBehaviour;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningOption;
import ProjetPoisson.mightylib.physics.tweenings.ETweeningType;
import ProjetPoisson.mightylib.physics.tweenings.type.Vector2fTweening;
import org.joml.Vector2f;

public class Fish {
    private final WindowInfo windowInfo;

    ETweeningBehaviour behaviour = ETweeningBehaviour.InOut;
    ETweeningType type = ETweeningType.Sinusoidal;
    ETweeningOption option = ETweeningOption.LoopMirrored;

    Vector2f position;
    Vector2fTweening goalPosition;

    private final RectangleRenderer renderer;

    public Fish(WindowInfo windowInfo, String textureName, Vector2f sizePercentage){
        this.windowInfo = windowInfo;

        renderer = new RectangleRenderer("texture2D");
        renderer.switchToTextureMode(textureName);
        renderer.setSizePix(windowInfo.getVirtualSizeCopy().x * sizePercentage.x,
                                windowInfo.getSizeCopy().y * sizePercentage.y);

        goalPosition = new Vector2fTweening();
        position = null;
    }

    public void update(){
        if (!goalPosition.finished()){
            goalPosition.update();

            if (goalPosition.finished()){
                renderer.setPosition(goalPosition.value());
            } else {
                renderer.setPosition(goalPosition.goalValue());
            }
        }
    }

    public boolean finishedTravel(){
        return goalPosition.finished();
    }
    public void display(){
        renderer.display();
    }

    public void travelToNewPosition(Vector2f positionPercentage, float time){
        Vector2f position = new Vector2f(windowInfo.getSizeCopy().x * positionPercentage.x,
                windowInfo.getSizeCopy().y * positionPercentage.y);

        if (this.position == null)
            this.position = position;
        else
            goalPosition.initRangeValue(time, this.position, position)
                    .setTweeningValues(type, behaviour)
                    .setTweeningOption(option);
    }

    public void unload(){
        renderer.unload();
    }
}
