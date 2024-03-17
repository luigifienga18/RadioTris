#Userlib directories
USERLIB = ./trisradio/3p/rf

# List of all the Userlib files
USERSRC =  $(USERLIB)/src/rf.c
          
# Required include directories
USERINC =  $(USERLIB) \
           $(USERLIB)/include 

# Shared variables
ALLCSRC += $(USERSRC)
ALLINC  += $(USERINC)