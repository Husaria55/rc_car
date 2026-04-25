import pygame
import socket
import time

# IP Robota (domyślne dla ESP32 AP)
UDP_IP = "192.168.4.1"
UDP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("BŁĄD: Nie widzę pada! Sparuj go najpierw z systemem.")
    exit()

pad = pygame.joystick.Joystick(0)
pad.init()
print(f"Sterowanie aktywne: {pad.get_name()}")

def get_speed(val):
    if abs(val) < 0.1: return 0 # Deadzone
    return int(-val * 255) # Odwrócenie osi (Y w dół to + w pygame)

try:
    while True:
        pygame.event.pump()
        
        # Oś 1 to lewa gałka Y, Oś 3 (lub 4/5 zależnie od systemu) to prawa gałka Y
        # Jeśli prawa gałka nie reaguje, sprawdź '3', '4' lub '5'
        l_speed = get_speed(pad.get_axis(1))
        r_speed = get_speed(pad.get_axis(3)) 

        # Przycisk bezpieczeństwa - np. Trójkąt (button 2 lub 3)
        if pad.get_button(0): 
            l_speed, r_speed = 0, 0

        msg = f"{l_speed},{r_speed}"
        sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
        
        time.sleep(0.02) # 50 Hz

except KeyboardInterrupt:
    sock.sendto("0,0".encode(), (UDP_IP, UDP_PORT))
    print("\nKoniec sterowania.")