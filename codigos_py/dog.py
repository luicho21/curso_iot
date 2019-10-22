## AUTHOR= Luis Salgueiro
## DATE=  21/10/2019
## DESCRIPTION = CODE FOR ACTIVITY 2 OF PLA3

class Dog:
    species="caniche"

    def __init__(self, name, age):
        self.name = name
        self.age = age


bambi = Dog("Bambi", 5)
mikey = Dog("Rufus", 6)
boby = Dog("Boby",15)
rino = Dog("Rino", 5)
oli = Dog("Oli", 8)
oto= Dog("Oto", 3)




#print("{} is {} and {} is {}".format(bambi.name, bambi.age, mikey.name, mikey.age))
#print("{} is {} and {} is {}".format(bambi.name, bambi.age, mikey.name, mikey.age))
#print("{} is {} and {} is {}".format(bambi.name, bambi.age, mikey.name, mikey.age))
print("I had three dogs")
print("{} and pased away at the age of {} ".format(boby.name, boby.age))
print("{} and pased away at the age of {} ".format(rino.name, rino.age))
print("{} and pased away at the age of {} ".format(oli.name, oli.age))
print("Know I have other dog which name is {}, it`s totally crazy".format(oto.name))


if bambi.species=="dcaniche":
    print("{0} is a {1}!".format(bambi.name, bambi.species))


def get_biggest_number():
    print("Please insert the list of integer numbers")
    s = input()
    a = list(map(int, s.split())) 
    print("You inserted this list", a)
    a.sort()
    print("The biggest value is: ",a[-1])


get_biggest_number()

