package ProjetPoisson.mightylib.graphics.text;

import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.mightylib.resources.DataType;
import ProjetPoisson.mightylib.resources.Resources;
import org.joml.Vector2f;
import org.joml.Vector4f;

public class FontFace extends DataType {
    private static final String PATH = "resources/textures/fonts/";
    private final String name;
    private final Texture fontAtlas;
    private final FontFile fontFile;

    FontFace(String fontFaceName, String textureName, String infoPath) {
        super(fontFaceName, PATH + infoPath);

        name = fontFaceName;
        fontAtlas = Resources.getInstance().getResource(Texture.class, textureName);

        fontFile = new FontFile(PATH + infoPath);
    }

    public Vector2f computeSize(String text, float fontSize){
        Vector2f result = new Vector2f();
        Vector2f currentCharOffset = new Vector2f();
        String[] temp = text.split("\n");

        for (String line : temp) {
            currentCharOffset.x = 0;

            for (int i = 0; i < line.length(); i++) {
                currentCharOffset.x += getFontFile().getCharacter(line.charAt(i)).getxAdvance() * fontSize;
            }

            result.x = Math.max(result.x, currentCharOffset.x);
        }

        currentCharOffset.x = 0;
        result.y += getFontFile().getLineHeight() * fontSize * temp.length;

        return result;
    }

    public String getName(){
        return name;
    }

    /**
     * Get font atlas texture.
     *
     * @return fontAtlas.
     */
    public Texture getTexture() {
        return fontAtlas;
    }

    /**
     * Get font file.
     *
     * @return fontFile.
     */
    public FontFile getFontFile() {
        return fontFile;
    }

    void setCorrectlyLoaded() { correctlyLoaded = true; }

    @Override
    public void unload() {
        correctlyLoaded = false;
    }
}
