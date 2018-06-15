/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package reproductor;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import static reproductor.initServer.SERVER;
import static reproductor.initServer.SOCKET_PORT;

/**
 *
 * @author curso
 */
public final class llamadaServer {
    
    public final static int SOCKET_PORT = 5000;
    public final static String SERVER = "192.168.1.100";  // localhost
    
    
    public llamadaServer() throws IOException {
        //init();
        conectar();
        System.out.println("Connecting...");
    }
    
    public void conectar() throws IOException{
        //Socket sock = new Socket(SERVER, SOCKET_PORT);
    }
    
    public void pedirLista(String loMandoPedir) throws IOException{
        int bytesRead;
        int current = 0;
        FileOutputStream fos = null;
        BufferedOutputStream bos = null;
        Socket sock = null;
        String File_Name = "";
        
        try {
            sock = new Socket(SERVER, SOCKET_PORT);
            System.out.println("Connecting...");
            BufferedWriter socketWriter = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
            BufferedReader socketReader = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));            
            socketWriter.write(loMandoPedir);
            socketWriter.flush();
            System.out.println("send file name");
            File_Name = loMandoPedir;
            // Read and display the size from the server
            InputStream size = sock.getInputStream();
            //String inMsg = socketReader.readLine();
            byte [] fsize  = new byte [50];
            int f = size.read(fsize,0,50);
            String finalSize = new String(fsize, "UTF-8").trim().replaceAll(" ", "");
            int FILE_SIZE = Integer.parseInt(finalSize);
            System.out.println("Server file size: " + (FILE_SIZE + 1));
            System.out.println(); // Print a blank line
            //System.out.print(promptMsg);
            socketWriter.write("crontrol msg");
            socketWriter.flush();
            // receive file
            System.out.println("file name: " + File_Name);
            byte [] mybytearray  = new byte [FILE_SIZE];
            InputStream is = sock.getInputStream();
            fos = new FileOutputStream(File_Name);
            bos = new BufferedOutputStream(fos);
            bytesRead = is.read(mybytearray,0,mybytearray.length);
            current = bytesRead;
            
            do {
                bytesRead = is.read(mybytearray, current, (mybytearray.length-current));
                if(bytesRead >= 0) current += bytesRead;
            } 
            while(current < FILE_SIZE);
                bos.write(mybytearray, 0 , current);
                bos.flush();
                System.out.println("File " + File_Name + " downloaded (" + current + " bytes read)");
            }
            
            finally {
                if (fos != null) fos.close();
                if (bos != null) bos.close();
                if (sock != null) sock.close();
            }
    
    }

    public void init() throws IOException {
        int bytesRead;
        int current = 0;
        FileOutputStream fos = null;
        BufferedOutputStream bos = null;
        Socket sock = null;
        String File_Name = "";
        
        try {
            sock = new Socket(SERVER, SOCKET_PORT);
            System.out.println("Connecting...");
            BufferedWriter socketWriter = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
            BufferedReader socketReader = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
            
            
            String promptMsg = "Please enter a  message: ";
            String outMsg = null;
            System.out.print(promptMsg);
            outMsg = consoleReader.readLine();
            socketWriter.write(outMsg);
            socketWriter.flush();
            System.out.println("send file name");
            File_Name = outMsg;
            // Read and display the size from the server
            InputStream size = sock.getInputStream();
            //String inMsg = socketReader.readLine();
            byte [] fsize  = new byte [50];
            int f = size.read(fsize,0,50);
            String finalSize = new String(fsize, "UTF-8").trim().replaceAll(" ", "");
            int FILE_SIZE = Integer.parseInt(finalSize);
            System.out.println("Server file size: " + (FILE_SIZE + 1));
            System.out.println(); // Print a blank line
            //System.out.print(promptMsg);
            socketWriter.write("crontrol msg");
            socketWriter.flush();
            // receive file
            System.out.println("file name: " + File_Name);
            byte [] mybytearray  = new byte [FILE_SIZE];
            InputStream is = sock.getInputStream();
            fos = new FileOutputStream(File_Name);
            bos = new BufferedOutputStream(fos);
            bytesRead = is.read(mybytearray,0,mybytearray.length);
            current = bytesRead;
            
            do {
                bytesRead = is.read(mybytearray, current, (mybytearray.length-current));
                if(bytesRead >= 0) current += bytesRead;
            } 
            while(current < FILE_SIZE);
                bos.write(mybytearray, 0 , current);
                bos.flush();
                System.out.println("File " + File_Name + " downloaded (" + current + " bytes read)");
            }
            
            finally {
                if (fos != null) fos.close();
                if (bos != null) bos.close();
                if (sock != null) sock.close();
            }
        
    }
    
}
