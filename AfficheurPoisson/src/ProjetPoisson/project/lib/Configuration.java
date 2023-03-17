package ProjetPoisson.project.lib;

import ProjetPoisson.mightylib.resources.DataType;

public class Configuration extends DataType {

    private String address;
    private String id;
    private int port;
    private String pathFishesResources;

    public Configuration(String dataName, String path) {
        super(dataName, path);
    }

    public void init(){

    }



    @Override
    public void unload() {

    }
}
