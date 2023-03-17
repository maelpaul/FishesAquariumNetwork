package ProjetPoisson.mightylib.graphics.renderer._2D;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.main.WindowInfo;

public class VirtualSceneRenderer extends RectangleRenderer {
    private final FrameBuffer frameBuffer;

    public VirtualSceneRenderer(WindowInfo info,  int frameBufferAspect){
        super("postProcessing");
        frameBuffer = new FrameBuffer(info, frameBufferAspect);
        this.shape.updateVbo(new float[]{
               -1, 1,
               -1, -1,
               1, -1,
               1, 1
        }, positionIndex);
    }

    public FrameBuffer getFrameBuffer(){
        return frameBuffer;
    }


    public void bindFrameBuff(){
        frameBuffer.bindFrameBuffer();
    }


    public void unbindFrameBuff(){
        frameBuffer.unbindFrameBuffer();
        frameBuffer.bindRenderTexture();
    }


    public void unload(){
        super.unload();
        frameBuffer.unload();
    }
}