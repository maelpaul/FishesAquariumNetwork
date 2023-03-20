package ProjetPoisson.mightylib.inputs;

import org.lwjgl.glfw.GLFW;

public abstract class KeyboardLanguage {
    protected KeyboardManager manager;
    public char translateKeyTo(int keyId){
        if (keyId >= GLFW.GLFW_KEY_A && keyId <= GLFW.GLFW_KEY_Z)
            return (char) ('a' + (keyId - GLFW.GLFW_KEY_A));

        if (keyId >= GLFW.GLFW_KEY_0 && keyId <= GLFW.GLFW_KEY_9)
            return (char)('0' + (keyId - GLFW.GLFW_KEY_0));

        if (keyId >= GLFW.GLFW_KEY_KP_0 && keyId <= GLFW.GLFW_KEY_KP_9)
            return (char)('0' + (keyId - GLFW.GLFW_KEY_KP_0));

        if (keyId == GLFW.GLFW_KEY_SPACE)
            return ' ';

        return (char)-1;
    }

    public abstract String keyboardConfigurationName();

    final protected boolean shouldCapsLock(){
        return manager.isCapsLock() || manager.getKeyState(GLFW.GLFW_KEY_LEFT_SHIFT) || manager.getKeyState(GLFW.GLFW_KEY_RIGHT_SHIFT);
    }

    protected abstract char toCapsLock(char c);

    void setManager(KeyboardManager m) {
        manager = m;
    }
}
