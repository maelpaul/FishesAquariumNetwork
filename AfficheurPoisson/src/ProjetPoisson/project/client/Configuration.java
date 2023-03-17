package ProjetPoisson.project.client;

import ProjetPoisson.mightylib.resources.DataType;

public class Configuration extends DataType {

    private String address;
    private String id;
    private int port;
    private String pathFishesResources;

    private int displayTimeout;

    private boolean set;

    public Configuration(String dataName, String path) {
        super(dataName, path);
    }

    public void init(String address, String id, int port, int displayTimeOut, String pathFishesResources){
        if (!set){
            this.address = address;
            this.id = id;
            this.port = port;
            this.displayTimeout = displayTimeOut;
            this.pathFishesResources = pathFishesResources;


            set = true;
        }
    }

    public String getAddress() { return address; }
    public String getId() { return id; }

    public int getPort() { return port; }

    public int getDisplayTimeout() { return displayTimeout; }
    public String getPathForFishesResources(){ return pathFishesResources; }

    @Override
    public void unload() {}
}
