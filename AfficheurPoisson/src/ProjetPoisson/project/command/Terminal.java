package ProjetPoisson.project.command;

import ProjetPoisson.mightylib.graphics.renderer._2D.shape.RectangleRenderer;
import ProjetPoisson.mightylib.graphics.text.ETextAlignment;
import ProjetPoisson.mightylib.graphics.text.Text;
import ProjetPoisson.mightylib.inputs.KeyboardManager;
import ProjetPoisson.mightylib.main.SystemInfo;
import ProjetPoisson.mightylib.physics.collision.CollisionRectangle;
import ProjetPoisson.mightylib.physics.collision.CollisionVisualisation;
import ProjetPoisson.mightylib.util.Timer;
import ProjetPoisson.mightylib.util.math.Color4f;
import ProjetPoisson.mightylib.util.math.EDirection;
import org.joml.Vector2f;
import org.joml.Vector3f;
import org.joml.Vector4f;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL;

public class Terminal {
    private final Vector2f referencePosition;
    private final Vector2f maxSize;

    public Text resultText;
    public Text commandText;

    private final Timer cursorTimer;
    private static final int CURSOR_PERIOD_TIME = 1;
    private boolean cursorDisplayed;

    private int cursorPosition;
    private final RectangleRenderer cursor;

    private StringBuilder commandTextCpy;

    public Terminal(Vector2f referencePosition, Vector2f maxSize){
        this.referencePosition = referencePosition;
        this.maxSize = maxSize;

        commandTextCpy = new StringBuilder("/");

        commandText = new Text();
        commandText.setFont("bahnschrift")
                .setAlignment(ETextAlignment.Left)
                .setReference(EDirection.LeftDown)
                .setPosition(new Vector2f(referencePosition))
                .setColor(new Color4f(0.2f, 0.2f, 0.2f, 1f))
                .setFontSize(30)
                .fitText(commandTextCpy.toString(), maxSize, true);

        resultText = new Text();
        commandText.copyTo(resultText)
                .setColor(new Color4f(0, 0, 0, 1f));

        updateResultPosition();

        cursorTimer = new Timer();
        cursorTimer.start(CURSOR_PERIOD_TIME);
        cursorDisplayed = false;

        cursor = new RectangleRenderer("colorShape2D");
        cursor.setScale(new Vector3f(3, commandText.getFontSize(), 1.f));
        cursor.switchToColorMode(new Color4f(0, 0, 0, 0.8f));
        cursor.setPosition(new Vector2f(37, 705));

        cursorPosition = 0;
    }

    private void updateResultPosition(){
        resultText.setPosition(new Vector2f(commandText.leftUpPosition().x, commandText.leftUpPosition().y));
    }

    public void update(KeyboardManager manager, SystemInfo info){
        boolean textUpdate = false;
        if (manager.keyPressed(GLFW.GLFW_KEY_V) && manager.getKeyState(GLFW.GLFW_KEY_LEFT_CONTROL)){
            addCharToPosition(cursorPosition + 1, info.getClipboardContent());
            textUpdate = true;
        } else if (manager.getMainKeyPressed() != -1 && manager.keyPressed(manager.getMainKeyPressed())) {
            char c = manager.getCorrespondingCharMainKeyPressed();

            if (c == ((char)-1)) {
                if ((manager.getMainKeyPressed() == GLFW.GLFW_KEY_RIGHT) && cursorPosition < commandText.text().length() - 1) {
                    updateCursorPosition(cursorPosition + 1);
                } else if ((manager.getMainKeyPressed() == GLFW.GLFW_KEY_LEFT) && cursorPosition > 0) {
                    updateCursorPosition(cursorPosition - 1);
                }
                // Todo touche suppr
                else if ((manager.getMainKeyPressed() == GLFW.GLFW_KEY_BACKSPACE ||
                        (manager.keyPressed(GLFW.GLFW_KEY_LEFT_SHIFT) && manager.keyPressed(GLFW.GLFW_KEY_KP_DECIMAL))
                )  && commandText.text().length() > 1){
                    removeCharAtPosition(cursorPosition);
                    textUpdate = true;
                } else if (manager.getMainKeyPressed() == GLFW.GLFW_KEY_ENTER
                        || manager.getMainKeyPressed() == GLFW.GLFW_KEY_KP_ENTER){
                    // Todo entrer commande
                }
            } else {
                addCharToPosition(cursorPosition + 1, c + "");
                textUpdate = true;
            }
        }

        if (textUpdate){
            updateResultPosition();
        }

        cursorTimer.update();
        if (cursorTimer.isFinished()){
            cursorDisplayed = !cursorDisplayed;
            cursorTimer.resetStart();
        }
    }

    private void addCharToPosition(int index, String chr){
        if (index < 1 || index > commandText.text().length())
            return;

        commandTextCpy.insert(index, chr);
        commandText.fitText(commandTextCpy.toString(), maxSize, true);

        updateCursorPosition(cursorPosition + chr.length());
    }

    private void removeCharAtPosition(int index){
        if (index < 1 || index > commandText.text().length() - 1)
            return;

        commandTextCpy.deleteCharAt(index);
        commandText.fitText(commandTextCpy.toString(), maxSize, true);

        updateCursorPosition(cursorPosition - 1);
    }

    private void updateCursorPosition(int index){
        cursorPosition = index;
        Vector4f charPosition = commandText.getPositionOfChar(cursorPosition);
        cursor.setPosition(new Vector2f((int)(charPosition.y) - commandText.getFontSize() * 0.15f, (int)(charPosition.z)));
        cursorTimer.resetStart();
        cursorDisplayed = true;
    }

    public void display(){
        commandText.display();
        resultText.display();

        if (cursorDisplayed) {
            cursor.display();
        }
    }

    public void unload(){
        commandText.unload();
        resultText.unload();


        cursor.unload();
    }
}
