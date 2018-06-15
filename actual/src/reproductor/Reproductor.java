package reproductor;

import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.AudioFileFormat;
import javax.sound.sampled.Clip;
import java.io.File;
import javazoom.jlgui.basicplayer.BasicPlayer;

public class Reproductor {
    
        public BasicPlayer player;

    public Reproductor() {
        player = new BasicPlayer();
    }    

    public void Play() throws Exception {
        player.play();
    }

    public void AbrirFichero(String ruta) throws Exception {
        player.open(new File(ruta));            
    }

    public void Pausa() throws Exception {
        player.pause();
    }

    public void Continuar() throws Exception {
        player.resume();
    }

    public void Stop() throws Exception {
        player.stop();
    }
    
    public void SetVolumen(double o) throws Exception {
        player.setGain(o);        
    }
    
    public int GetEstado() throws Exception {
        int valor;
        valor =  player.getStatus();
        return valor;//devuelve 0 si la cancion esta reproduciendo y 1 si esta en pausa
    }
    
    public float GetAvance() throws Exception {
        float valor;
        valor =  player.getGainValue();       
        
        return valor;//devuelve 0 si la cancion esta reproduciendo y 1 si esta en pausa
    }
}
