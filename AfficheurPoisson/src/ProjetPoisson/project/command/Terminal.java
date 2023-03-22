package ProjetPoisson.project.command;

import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.inputs.KeyboardManager;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import org.joml.Vector2f;
import org.lwjgl.glfw.GLFW;

public class Terminal {
    private Vector2f referencePosition, maxSize;

    public Text resultText;
    public Text commandText;

    public Terminal(Vector2f referencePosition, Vector2f maxSize){
        this.referencePosition = referencePosition;
        this.maxSize = maxSize;

        commandText = new Text();
        commandText.setFont("bahnschrift")
                .setAlignment(ETextAlignment.Left)
                .setReference(EDirection.LeftDown)
                .setPosition(new Vector2f(referencePosition))
                .setColor(new Color4f(0.2f, 0.2f, 0.2f, 1f))
                .setFontSize(30)
                .fitText("/", maxSize, true);

        resultText = new Text();
        commandText.copyTo(resultText)
                .setColor(new Color4f(0, 0, 0, 1f));

        updateResultPosition();
    }

    private void updateResultPosition(){
        resultText.setPosition(new Vector2f(commandText.leftUpPosition().x, commandText.leftUpPosition().y));
    }

    public void update(KeyboardManager manager){
        boolean textUpdate = false;
        if (manager.getMainKeyPressed() != -1 && manager.keyPressed(manager.getMainKeyPressed())) {
            char c = manager.getCorrespondingCharMainKeyPressed();

            if (c == ((char)-1)) {
                if (manager.getMainKeyPressed() == GLFW.GLFW_KEY_BACKSPACE && commandText.text().length() > 1){
                    commandText.fitText(commandText.text().substring(0, commandText.text().length() - 1), maxSize, true);
                    textUpdate = true;
                } else if (manager.getMainKeyPressed() == GLFW.GLFW_KEY_ENTER
                        || manager.getMainKeyPressed() == GLFW.GLFW_KEY_KP_ENTER){
                    // Todo entrer commande
                } else if (manager.getMainKeyPressed() == GLFW.GLFW_KEY_KP_DECIMAL){
                    // Todo touche suppr
                }
            } else {
                commandText.fitText(commandText.text() + c, maxSize, true);
                textUpdate = true;
            }
        }

        if (textUpdate){
            updateResultPosition();
        }
    }

    public void display(){
        commandText.display();
        resultText.display();
    }

    public void unload(){
        commandText.unload();
        resultText.unload();
    }
}
