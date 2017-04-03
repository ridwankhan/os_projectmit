[1mdiff --git a/homework2/problem1/problem1.c b/homework2/problem1/problem1.c[m
[1mindex 4c0556f..e9feec3 100644[m
[1m--- a/homework2/problem1/problem1.c[m
[1m+++ b/homework2/problem1/problem1.c[m
[36m@@ -47,9 +47,9 @@[m [mvoid reversePrime(int myArray[]){[m
 			n = i;[m
 			reverse = 0;[m
 			while (n != 0){[m
[31m-			reverse = reverse*10;[m
[31m-			reverse = reverse + n%10;[m
[31m-			n = n/10;[m
[32m+[m				[32mreverse = reverse*10;[m
[32m+[m				[32mreverse = reverse + n%10;[m
[32m+[m				[32mn = n/10;[m
 			}[m
 			if ((reverse <= number) && myArray[reverse])[m
 			{[m
[36m@@ -93,7 +93,7 @@[m [mint main(int argc, char *argv[]){[m
 	pthread_attr_init(&attr2);[m
 	[m
 	pthread_create(&tid2, &attr2, (void*)reversePrime, &myArray);[m
[31m-	 pthread_join(tid2,NULL);[m
[32m+[m	[32mpthread_join(tid2,NULL);[m
 	[m
 	[m
 	[m
