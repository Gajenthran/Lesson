#lang racket/base

(require parser-tools/lex
         parser-tools/yacc)

(define-tokens toks
  (La Ln))

(define-empty-tokens etoks
  (Lw Lend Lopar Lcpar))


(define tokens
  (lexer
    ((eof)        (token-Lend))
    ("("          (token-Lopar))
    (")"          (token-Lcpar))
    (alphabetic   (token-La lexeme))
    (numeric      (token-Ln (string->number lexeme)))
    (whitespace   (token-Lw))))


(define parse1
  (parser
    (tokens toks etoks)
    (grammar
      (expr
        ((expr Lw instr)         'ok)
        ((instr)                 'ok))
      (instr
        ((num)                   null)
        ((word)                  null))
      (num
        ((Ln)                    null)
        ((num Ln)                null))
      (word
        ((La)                    null)
        ((word La)               null)))
    (start expr)
    (end Lend)
    (error (lambda (ok? name value)
            (error (if value value 'Error)
                   (if ok? "error" "invalid token"))))))


(define parse2
  (parser
    (tokens toks etoks)
    (grammar
      (expr
        ((expr Lw instr)         (append $1 $3))
        ((instr)                 $1))
      (instr
        ((num)                   (list $1))
        ((word)                  (list $1)))
      (num
        ((Ln)                    $1)
        ((num Ln)                (+ $1 $2)))
      (word
        ((La)                    $1)
        ((word La)               (string-append $1 $2))))
    (start expr)
    (end Lend)
    (error (lambda (ok? name value)
            (error (if value value 'Error)
                   (if ok? "error" "invalid token"))))))


(define parse3
  (parser
    (tokens toks etoks)
    (grammar
      (expr
        ((expr Lw instr)         (append $1 $3))
        ((instr)                 $1))
      (instr
        ((Lopar expr Lcpar)      (list $2))
        ((num)                   (list $1))
        ((word)                  (list $1)))
      (num
        ((Ln)                    $1)
        ((num Ln)                (+ $1 $2)))
      (word
        ((La)                    $1)
        ((word La)               (string-append $1 $2))))
    (start expr)
    (end Lend)
    (error (lambda (ok? name value)
            (error (if value value 'Error)
                   (if ok? "error" "invalid token"))))))


(define argv (current-command-line-arguments))
(cond
  ((= (vector-length argv) 1)
    (define in (open-input-string (vector-ref argv 0)))
    (write (parse3 (lambda () (tokens in))))
    (newline)
    (close-input-port in)
    (exit 0))
  (else
    (eprintf "Usage: racket parsers.rkt \"string\"\n")
    (exit 1)))  
