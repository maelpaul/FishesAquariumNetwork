package ProjetPoisson.project.lib;

import ProjetPoisson.mightylib.resources.DataType;
import ProjetPoisson.mightylib.resources.ResourceLoader;
import ProjetPoisson.mightylib.resources.map.TileMap;

import java.util.Map;

public class ConfigurationLoader extends ResourceLoader {
    @Override
    public Class<?> getType() {
        return Configuration.class;
    }

    @Override
    public String getResourceNameType() {
        return "Configuration";
    }

    @Override
    public void create(Map<String, DataType> data) {
        // TODO
    }

    @Override
    public void load(DataType dataType) {
        if (!(dataType instanceof Configuration))
            return;

        Configuration configuration = (Configuration) dataType;

        // TODO

        // configuration.init(... , ...);
    }
}
