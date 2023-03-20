package ProjetPoisson.mightylib.inputs.keyboardlanguage;

import ProjetPoisson.mightylib.inputs.KeyboardLanguage;

import java.util.HashMap;

public class NativeKeyboardLanguage extends KeyboardLanguage {
    private static final KeyboardLanguage instance = new NativeKeyboardLanguage();

    private HashMap<Character, Character> capslock;

    public static KeyboardLanguage getInstance() { return instance; }
    private NativeKeyboardLanguage(){
        capslock = new HashMap<>();
        capslock.put('1', '!');
        capslock.put('2', '@');
        capslock.put('3', '#');
        capslock.put('4', '$');
        capslock.put('5', '%');
        capslock.put('6', '^');
        capslock.put('7', '&');
        capslock.put('8', '*');
        capslock.put('9', '(');
        capslock.put('0', ')');
        capslock.put('-', '_');
        capslock.put('=', '+');
        capslock.put('[', '{');
        capslock.put(']', '}');
        capslock.put('\'', '\"');
        capslock.put('\\', '|');
        capslock.put('m', '/');
        capslock.put(',', '<');
        capslock.put('.', '>');
        capslock.put('/', '?');
        capslock.put('`', '~');
    }

    @Override
    public char translateKeyTo(int keyId) {
        return super.translateKeyTo(keyId);
    }

    @Override
    public String keyboardConfigurationName() {
        return "qwerty";
    }

    @Override
    protected char toCapsLock(char c) {


        return c;
    }
}
