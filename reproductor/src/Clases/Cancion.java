
package Clases;

public class Cancion {
    
    private int id;
    private String nombre;
    private long tamaño;
    private String direccion;

    public Cancion(int id, String nombre, long tamaño, String direccion) {
        this.id = id;
        this.nombre = nombre;
        this.tamaño = tamaño;
        this.direccion = direccion;
    }

    public Cancion() {
    }

    public int getId() {
        return id;
    }

    public String getDireccion() {
        return direccion;
    }

    public void setDireccion(String direccion) {
        this.direccion = direccion;
    }


    public void setId(int id) {
        this.id = id;
    }

    public long getTamaño() {
        return tamaño;
    }

    public String getNombre() {
        return nombre;
    }


    public void setTamaño(long tamaño) {
        this.tamaño = tamaño;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }
}
