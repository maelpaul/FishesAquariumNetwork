package ProjetPoisson.mightylib.inputs.keyboardlanguage;

import ProjetPoisson.mightylib.inputs.KeyboardLanguage;
import static org.lwjgl.glfw.GLFW.*;

import java.util.HashMap;

public class AZERTYKeyboardLanguage extends KeyboardLanguage {
    private static final KeyboardLanguage instance = new AZERTYKeyboardLanguage();

    public static KeyboardLanguage getInstance() { return instance; }

    private HashMap<Character, Character> relation;

    private HashMap<Character, Character> capslock;

    private HashMap<Integer, Character> entries;
    private AZERTYKeyboardLanguage(){
        relation = new HashMap<>();
        relation.put('q', 'a');
        relation.put('w', 'z');
        relation.put('a', 'q');
        relation.put('z', 'w');
        relation.put('m', ',');

        entries = new HashMap<>();
        entries.put(GLFW_KEY_LEFT_BRACKET, '^');
        entries.put(GLFW_KEY_RIGHT_BRACKET, '$');
        entries.put(GLFW_KEY_GRAVE_ACCENT, '²');
        entries.put(GLFW_KEY_SEMICOLON, 'm');
        entries.put(GLFW_KEY_APOSTROPHE, 'ù');
        entries.put(GLFW_KEY_BACKSLASH, '*');
        entries.put(GLFW_KEY_COMMA, ';');
        entries.put(GLFW_KEY_PERIOD, ':');
        entries.put(GLFW_KEY_SLASH, '!');
        entries.put(GLFW_KEY_MINUS, ')');
        entries.put(GLFW_KEY_EQUAL, '=');

        entries.put(GLFW_KEY_KP_DIVIDE, '/');
        entries.put(GLFW_KEY_KP_MULTIPLY, '*');
        entries.put(GLFW_KEY_KP_SUBTRACT, '-');
        entries.put(GLFW_KEY_KP_ADD, '+');
        entries.put(GLFW_KEY_KP_DECIMAL, '.');

        capslock = new HashMap<>();
        capslock.put('&', '1');
        capslock.put('é', '2');
        capslock.put('\"', '3');
        capslock.put('\'', '4');
        capslock.put('(', '5');
        capslock.put('-', '6');
        capslock.put('è', '7');
        capslock.put('_', '8');
        capslock.put('ç', '9');
        capslock.put('à', '0');
        capslock.put(')', '°');
        capslock.put('=', '+');
        capslock.put('^', '¨');
        capslock.put('$', '£');
        capslock.put('ù', '%');
        capslock.put('*', 'µ');
        capslock.put(',', '?');
        capslock.put(';', '.');
        capslock.put(':', '/');
        capslock.put('!', '§');
        capslock.put('²', (char)-1);
        capslock.put('<', '>');

        capslock.put('.', (char)-1);
    }

    @Override
    public char translateKeyTo(int keyId) {
        char result = super.translateKeyTo(keyId);
        if (relation.containsKey(result))
            result = relation.get(result);

        if (entries.containsKey(keyId))
            result = entries.get(keyId);

        if (shouldCapsLock())
            result = toCapsLock(result);

        return result;
    }

    @Override
    public String keyboardConfigurationName() {
        return "azerty";
    }

    @Override
    protected char toCapsLock(char c) {
        if (c >= 'a' && c <= 'z')
            return (char)('A' + (c - 'a'));

        if (capslock.containsKey(c))
            return capslock.get(c);

        return c;
    }
}
