#lang racket/base

(require parser-tools/lex
         (prefix-in : parser-tools/lex-sre))

(define-empty-tokens operators
  (Eof
  Lambda Dot
  POpen PClose))

(define-tokens identifiers
  (Var))

(define get-token
  (lexer
    ((eof)           (token-Eof))
    (whitespace      (get-token input-port))
    ("λ"             (token-Lambda))
    ("."             (token-Dot))
    ("("             (token-POpen))
    (")"             (token-PClose))
    ((:+ alphabetic) (token-Var lexeme))
    (any-char        (error (format "Unrecognized char '~a' at offset ~a."
                                   lexeme (position-offset start-pos))))))

(define current-token null)
(define in null)

;; Non-terminaux

(define (match-expr)
  (cond
    ((eq? 'POpen (token-name current-token))
      (match-t 'POpen)
      (match-fact)
      (match-t 'PClose))
    (else
      (match-var))))

(define (match-fact)
  (cond
    ((eq? 'Lambda (token-name current-token))
      (match-t 'Lambda)
      (match-var)
      (match-t 'Dot)
      (match-expr))
    (else
      (match-expr)
      (match-expr))))


;; Terminaux

(define (match-var)
  (cond
    ((eq? 'Var (token-name current-token))
      (display (token-value current-token))
      (display " ")
      (set! current-token (get-token in)))
    (else
      (error 'parser "Expected Var\n"))))

(define (match-t t)
  (cond
    ((eq? t (token-name current-token))
      (unless (eq? t 'Eof)
        (cond
          ((eq? t 'Dot)
            (display "."))
          ((eq? t 'POpen)
            (display "("))
          ((eq? t 'PClose)
            (display ")"))
          ((eq? t 'Lambda)
            (display "λ"))
          (else
            (display t)))
        (display " "))
      (set! current-token (get-token in)))
    (else
      (error 'parser "Expected ~a\n" t))))


(define argv (current-command-line-arguments))
(cond
  ((= (vector-length argv) 1)
    (set! in (open-input-string (vector-ref argv 0)))
    (set! current-token (get-token in))
    (match-expr)
    (match-t 'Eof))  
  (else
    (eprintf "Usage: racket lc-match.rkt \"string\"\n")
    (exit 1)))
