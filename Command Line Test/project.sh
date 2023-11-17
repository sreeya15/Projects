#!/bin/bash
<<doc
Name:Sreeya L Karthik
Date: 25 February 2023
Title: Command Line Test
Sample I/P:
Sample O/P:
doc

start="y"
while [ $start == "y" ]
do
    echo "1. Sign Up"
    echo "2. Sign In"
    echo "3. Exit"

    user_arr=(`cat username.csv`)
    passwordarr=(`cat password.csv`)

    read -p "Enter the option 1, 2 or 3 =  " option

    case $option in
	1)
	    var=0
	    while [ $var == 0 ]
	    do
		var=1
		read -p " Enter the UserName = " user

		
		for i in ${user_arr[@]}
		do
		    if [ $user == $i ]
		    then
			var=0
			echo "UserName Already exits"
			break
		    fi
		done
	    done

	    flag=0
	    while [ $flag == 0 ]
	    do
		echo "Enter the Password = "
		read -s password1
		echo  "Confirm the Password = " 
		read -s password2
		if [ $password1 == $password2 ]
		then
		    echo
		    echo "Thank you..Account is successfully created"
		    flag=1
		    break
		else
		    echo "Entered password is wrong"
		    flag=0
		fi
	    done

	    echo $user >> username.csv
	    echo $password1 >> password.csv
	    ;;
	2)
	    count=0
	    while [ $count == 0 ]
	    do
		read -p "Enter the UserName : " user
		
		for i in `seq 0 $((${#user_arr[@]}-1))`
		do
		    if [ $user == ${user_arr[$i]} ]
		    then
			count=1
			x=$i
			break
		    fi
		done

		if [ $count == 0 ]
		then
		    echo "Invalid user"
		fi
	    done

	    var1=0
	    while [ $var1 == 0 ]
	    do
		read -s -p "Enter the Password : " pass
		if [ $pass == ${passwordarr[$x]} ]
		then
		    echo
		    echo "Thank you..You have been successfully Logged In."
		    var1=1
		    break
		else
		    echo "Password is not matching."
		    var1=0
		fi
	    done

	    echo "1. Take a test"
	    echo "2. Exit"
	    echo
	    read -p "Select option : " option
	    case $option in
		1)
		    for i in `seq 5 5 50`
		    do
			head -$i question.txt | tail -5
			for j in `seq 10 -1 1`
			do
			    echo -ne "\rThe choice is : $j " 
			    read -t 1 choice
			    if [ -n "$choice" ]
			    then
				break
			    else
				choice=e
			    fi
			done
			echo $choice >> userAnswer.txt
		    done
		    user_ans=(`cat userAnswer.txt`)
		    correct_ans=(`cat answers.txt`)

		    marks=0
		    index=0
		    for k in `seq 5 5 50`
		    do
			head -$k question.txt | tail -5
			echo "Your answer is ${user_ans[$index]}"
			echo
			echo "Correct Answer is ${correct_ans[$index]}"

			if [ ${user_ans[$index]} == ${correct_ans[$index]} ]
			then
			    echo "Your answer is correct"
			    echo
			    marks=$(($marks+1))
			   
			 
			elif [ ${user_ans[$index]} == "e" ]
			then
			    echo timeout
			else
			    echo "Entered wrong answer"
			    echo
			fi
			index=$(($index+1))
		    done
		    echo "Totalmarks=$marks"
		    rm userAnswer.txt
		    ;;
		2)
		    exit
		    ;;
		*)
		    echo "Invalid option"
		    ;;
	    esac
	    ;;
	3)
	    exit
	    ;;
	*)
	    echo "Invalid option"
	    ;;
    esac
    read -p "Do you want to continue(y/n) " start
done
