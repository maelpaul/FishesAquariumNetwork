package ProjetPoisson.mightylib.resources.animation;

import ProjetPoisson.mightylib.resources.DataType;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.Texture;

public class AnimationData extends DataType {

    private FrameData[] framesData;
    private String textureName;


    public AnimationData(String name, String path){
        super(name, path);

        textureName = "error";
    }


    public AnimationData setTexture(String texture){
        textureName = texture;

        checkLoaded();
        return this;
    }


    public AnimationData setFramesData(FrameData[] framesData){
        this.framesData = framesData;

        checkLoaded();
        return this;
    }

    private void checkLoaded(){
        correctlyLoaded = framesData != null && Resources.getInstance().isExistingResource(Texture.class, textureName);
    }


    public FrameData getFrame(int index){
        if (framesData == null)
            return null;
        if (0 > index || index > frameNumber())
            return null;

        return framesData[index];
    }


    public int frameNumber(){
        return framesData.length;
    }


    public String getTextureName(){ return textureName; }


    @Override
    public void unload(){
        textureName = "error";
        framesData = null;

        correctlyLoaded = false;
    }
}
