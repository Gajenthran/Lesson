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


(define argv (current-command-line-arguments))
(cond
  ((= (vector-length argv) 1)
    (define in (open-input-string (vector-ref argv 0)))
    (write (parse1 (lambda () (tokens in))))
    (newline)
    (close-input-port in)
    (exit 0))
  (else
    (eprintf "Usage: racket parsers.rkt \"string\"\n")
    (exit 1)))  
