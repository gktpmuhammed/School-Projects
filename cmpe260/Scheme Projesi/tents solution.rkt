#lang scheme
; 2017400162

;You can replace #f's with your function definitions and define more helper functions as you need to use this template.

; Solver function
(define TENTS-SOLUTION #f)

(define REPLACE-NTH (lambda (liste n newVal) ( cond [(= n 1) (cons newVal (cdr liste))]
                                                    [else (cons (car liste) (REPLACE-NTH (cdr liste) (- n 1) newVal))])))

; Helper functions
(define RETURN-FIRST-NOT-FALSE (lambda (f valList) (if(null? valList) #f (if(f (car valList)) (f (car valList)) (RETURN-FIRST-NOT-FALSE f (cdr valList)) )) ))

(define ADJACENT (lambda (list1 list2) (if(=(-(car list1)(car list2)) 1) (if(=(-(cadr list1)(cadr list2)) 1) #t (if(=(-(cadr list1)(cadr list2)) 0) #t (if(=(-(cadr list1)(cadr list2)) -1) #t #f)))
                                          (if(=(-(car list1)(car list2)) -1) (if(=(-(cadr list1)(cadr list2)) 1) #t (if(=(-(cadr list1)(cadr list2)) 0) #t (if(=(-(cadr list1)(cadr list2)) -1) #t #f)))
                                          (if(=(-(car list1)(car list2)) 0) (if(=(-(cadr list1)(cadr list2)) 1) #t (if(=(-(cadr list1)(cadr list2)) 0) #t (if(=(-(cadr list1)(cadr list2)) -1) #t #f))) #f)))))


(define ADJACENT-WITH-LIST (lambda (cord listCord) (if(null? listCord) #f (if(ADJACENT cord (car listCord)) #t (ADJACENT-WITH-LIST cord (cdr listCord))))))

(define left (lambda (liste)(cons(+ 1 (car liste)) (cdr liste))))

(define right (lambda (liste)(cons(- (car liste) 1) (cdr liste))))

(define up (lambda (liste)(cons(car liste) (cons(+ (cadr liste) 1) '()))))

(define down (lambda (liste)(cons(car liste) (cons(- (cadr liste) 1) '()))))

(define NEIGHBOR-LIST (lambda (liste) (cons (left liste) (cons (right liste) (cons(up liste) (cons(down liste) '()))))))


