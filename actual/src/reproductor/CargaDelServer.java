package reproductor;

import Clases.Cancion;
import InterfaceReproductor.Caratula;
import static InterfaceReproductor.Caratula.jList1;
import java.io.File;
import static InterfaceReproductor.Caratula.jSlider1;
import java.io.BufferedReader;
import java.io.FileReader;
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
    }
        
       
    public void lecturaArchivo(){
        File archivo = null;
        FileReader fr = null;
        BufferedReader br = null;
      try {
         // Apertura del fichero y creacion de BufferedReader para poder
         // hacer una lectura comoda (disponer del metodo readLine()).
         archivo = new File ("E:/re/reproductor/songs.txt");
         fr = new FileReader (archivo);
         br = new BufferedReader(fr);

         // Lectura del fichero
         String linea;
         while((linea=br.readLine())!=null){
         //System.out.println(linea);
            ListaCanciones.add(linea);
         }
         cargaCancionesDesdeLista(ListaCanciones);   
      }
      
      catch(Exception e){
         e.printStackTrace();
      }finally{
         // En el finally cerramos el fichero, para asegurarnos
         // que se cierra tanto si todo va bien como si salta 
         // una excepcion.
         try{                    
            if( null != fr ){   
               fr.close();     
            }                  
         }catch (Exception e2){ 
            e2.printStackTrace();
         }
      }  
    
    }
    
    public void cargaCancionesDesdeLista(ArrayList pLista){ //aca es donde deberia cargar la lista de nombres q viene desde el server       
        DefaultListModel listModel = new DefaultListModel();
        for(int i=0; i<pLista.size(); i++) {
             listModel.add(i, pLista.get(i));
        }
        //Asociar el modelo de lista al JList
        jList1.setModel(listModel);
                
    }
    
    public void CargaArchivoLocal(String pCancion ,int id) throws Exception{        
        File f = new File("E:/re/reproductor/"+pCancion);//+".mp3");
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
            Logger.getLogger(Caratula.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    }
    
    public void SlideVolumen(){
        float Vol = (float) (jSlider1.getValue()*0.01);                
        try {
            playerSO.SetVolumen(Vol);
        } catch (Exception ex) {            
            Logger.getLogger(Caratula.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    }
    
    public void BtonMute(){
    float Vol = (float) 0.0;
        try {
            playerSO.SetVolumen(Vol);
        } catch (Exception ex) {            
            Logger.getLogger(Caratula.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

   
}
