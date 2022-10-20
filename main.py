from cgi import print_form
import keyboard as kb
import threading
import time
import random
import queue

SILLAS = 3
ESPERAS = 1
sala_espera = queue.Queue(SILLAS)

#Simular el tiempo de llegada 
def espera():
    time.sleep(ESPERAS)

class Barbero(threading.Thread):
    condicion = threading.Condition() #se despierta o duerme al barbero
    alto_completo = threading.Event() #se detiene al barbero cuando todos los clientes han sido atendidos

    def __init__(self,ID):
        super().__init__()
        self.ID = ID
    
    def run(self):
        while True:
            try: #Intentar recuperar un cliente de la sala de espera
                cliente_actual = sala_espera.get(block=False) #Recupera al cliente y por ende al hilo y lo desbloquea
            except queue.Empty: #Si no existen clientes esperando por su corte 
                if self.alto_completo.is_set(): #Se activa cuando ya no existen clientes por atender
                    return
            else:
                cliente_actual.cortar(self.ID)

class Cliente (threading.Thread):
    duracion_corte = random.randint(1,5)

    def __init__(self, ID):
        super().__init__()
        self.duracion_corte = random.randint(1,5)
        self.ID = ID
    
    def corte(self): #Función para dormir el proceso mientras el barbero corta el pelo
        time.sleep(self.duracion_corte)

    def cortar(self, id_barbero): #Accion de que el barbero le corte el cabello
        print(f"El barbero {id_barbero} está cortando el cabello del cliente {self.ID}")
        self.corte()
        print(f"El Barbero {id_barbero} terminó de cortar el cabello al cliente {self.ID}")
        self.atendido.set() #Ya que fue atendido el cliente, se desbloquea el proceso para terminar su ejeución
    
    def run(self):
        self.atendido = threading.Event()

        try: #Revisar si existe espacio en la sala de espera
            sala_espera.put(self, block=False)
        except queue.Full:  #Si esta llena el cliente se va ç
            print(f"La sala de espera esta llena, el cliente {self.ID} se fue...")
            return
        #Si la sala no esta llena se siente y notifica al barbero
        print(f"El cliente {self.ID} se sentó en la sala de espera")
        with Barbero.condicion:
            Barbero.condicion.notify(1)
        
        self.atendido.wait() #Bloquea el proceso hasta ser atendido

    
def main():
    TODOS_CLIENTES = []

    hilo_barbero = Barbero(1)
    hilo_barbero.start()
    
    num_cliente = 1
    while True:
        if kb.is_pressed('Esc'):
            Barbero.alto_completo.set()
            with Barbero.condicion:
                Barbero.condicion.notify_all()
                print("La Barbería está cerrada por hoy, terminando de atender a los pendientes")
            return
        cliente = Cliente(num_cliente)
        num_cliente += 1
        TODOS_CLIENTES.append(cliente)
        cliente.start()
        espera()

if __name__ == "__main__":
    main()