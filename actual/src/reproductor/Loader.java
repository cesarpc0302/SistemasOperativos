package reproductor;

import InterfaceReproductor.*;
import java.io.IOException;

public class Loader{   

    public void loader() throws IOException{
       login pLogin = new login();         
       pLogin.setVisible(true);
      //llamadaServer server = new llamadaServer();
      //server.conectar();
      //server.pedirLista("songs.txt");
      //server.pedirLista("pink_floyd-another_brick_in_the_wall.mp3");
    }

}
