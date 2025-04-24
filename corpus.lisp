(defun carre (x)
  (* x x))

(defun somme-carres (a b)
  (+ (carre a) (carre b)))

(defun factorielle (n)
  (if (= n 0)
      1
      (* n (factorielle (- n 1)))))

(defun somme-liste (lst)
  (if (null lst)
      0
      (+ (car lst) (somme-liste (cdr lst)))))

(defun est-pair (n)
  (= (% n 2) 0))

(print (somme-carres 3 4))
(print (factorielle 5))
(print (somme-liste '(1 2 3 4 5)))
(print (est-pair 10))
