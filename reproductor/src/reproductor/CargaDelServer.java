package reproductor;

import Clases.Cancion;
import InterfaceReproductor.NewJFrame;
import static InterfaceReproductor.NewJFrame.jList1;
import java.io.File;
import static InterfaceReproductor.NewJFrame.jSlider1;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultListModel;

public class CargaDelServer {    
    private ArrayList lista;
    private Cancion cancion; 
    
    public static ArrayList <String> ListaCanciones;
    public Reproductor playerSO; 
    
    public CargaDelServer() {
        init();
    }

    public void init() {
        
        ListaCanciones = new ArrayList();
        playerSO = new Reproductor();        
        lista = new ArrayList();
        String f = "UFO";
        String f2 = "Cancion 2";
        String f3 = "Cancion 3";
        String f4 = "Cancion 4";
        String f5 = "mia";
        String f6 = "Derroche";
        ListaCanciones.add(f);        
        ListaCanciones.add(f2);
        ListaCanciones.add(f3);
        ListaCanciones.add(f4);
        ListaCanciones.add(f5);
        ListaCanciones.add(f6);
    }
        
    public void cargaCancionesDesdeLista(){ //aca es donde deberia cargar la lista de nombres q viene desde el server       
        DefaultListModel listModel = new DefaultListModel();
        for(int i=0; i<ListaCanciones.size(); i++) {
             listModel.add(i, ListaCanciones.get(i));
        }
        //Asociar el modelo de lista al JList
        jList1.setModel(listModel); 
                
    }
    
    public void CargaArchivoLocal(String pCancion ,int id) throws Exception{        
        File f = new File("D:/mp3/"+pCancion+".mp3");
        cancion = new Cancion(id, f.getName(), f.length(), f.getAbsolutePath());
        lista.add(cancion);
        System.out.println("CargaArchivoLocal este "+f.getName() +"tad" +f.getAbsolutePath());
        try {            
            playerSO.AbrirFichero(f.getAbsolutePath());          
            
        }catch (Exception ex){
            System.out.println("Error: " + ex.getMessage());
        }
        playerSO.Play();
    }
    
    public void botnPlay(){
        try {
            if (playerSO.GetEstado() == 3) {
                playerSO.Play();                
            }
            if (playerSO.GetEstado() == 1) {
                playerSO.Continuar();                
            }
        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
        }    
    
    }
    
    public void botnPausa(){
        try {            
            playerSO.Pausa();
        } catch (Exception ex) {
            Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    }
    
    public void SlideVolumen(){
        float Vol = (float) (jSlider1.getValue()*0.01);                
        try {
            playerSO.SetVolumen(Vol);
        } catch (Exception ex) {            
            Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    }
    
    public void BtonMute(){
    float Vol = (float) 0.0;
        try {
            playerSO.SetVolumen(Vol);
        } catch (Exception ex) {            
            Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

   
}
