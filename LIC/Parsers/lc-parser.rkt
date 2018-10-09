#lang racket/base

(require parser-tools/lex
         (prefix-in : parser-tools/lex-sre)
         parser-tools/yacc
         racket/port)

(define-empty-tokens operators
  (Eof
  Lambda Dot
  POpen PClose))

(define-tokens identifiers
  (Var))


(define tokens
  (lexer-src-pos
    ((eof)           (token-Eof))
    (whitespace      (return-without-pos (tokens input-port)))
    ("λ"             (token-Lambda))
    ("."             (token-Dot))
    ("("             (token-POpen))
    (")"             (token-PClose))
    ((:+ alphabetic) (token-Var lexeme))
    (any-char        (error (format "Unrecognized char '~a' at offset ~a."
                                   lexeme (position-offset start-pos))))))


(struct Var (name) #:transparent)
(struct Abs (lambda var-name dot expr) #:transparent)
(struct App (popen expr1 expr2 pclose) #:transparent)


(define lc-parse
  (parser
    (src-pos)
    (tokens identifiers operators)
    (grammar
      (expr
        ((vars)                    $1)
        ((Lambda vars Dot expr)    (list (Abs "λ" $2 "." $4)))
        ((POpen expr expr PClose)  (list (App "(" $2 $3 ")"))))
      (vars
        ((Var)                     (Var $1))))
    (start expr)
    (end Eof)
    (error (lambda (ok? tok-name tok-value start-pos end-pos)
            (error (format "Error at token ~a with value : ~a [~a - ~a]." 
                    tok-name tok-value start-pos end-pos))))))


(define argv (current-command-line-arguments))
(cond
  ((= (vector-length argv) 1)
    (define in (open-input-string (vector-ref argv 0)))
    (write (lc-parse (lambda () (tokens in))))
    (newline)
    (close-input-port in)
    (exit 0))
  (else
    (eprintf "Usage: racket parsers.rkt \"string\"\n")
    (exit 1)))  
