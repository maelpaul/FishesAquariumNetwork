package ProjetPoisson.project.display;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.main.WindowInfo;
import org.joml.Vector2f;

public class Fish {
    private final WindowInfo windowInfo;

    private final RectangleRenderer renderer;

    public Fish(WindowInfo windowInfo, String textureName, Vector2f sizePercentage){
        this.windowInfo = windowInfo;

        renderer = new RectangleRenderer("texture2D");
        renderer.switchToTextureMode(textureName);
        renderer.setSizePix(windowInfo.getVirtualSizeCopy().x * sizePercentage.x,
                                windowInfo.getSizeCopy().y * sizePercentage.y);
    }


    public void setPositionPercentage(Vector2f positionPercentage){
        renderer.setPosition(
                new Vector2f(windowInfo.getSizeCopy().x * positionPercentage.x,
                            windowInfo.getSizeCopy().y * positionPercentage.y));
    }
}
