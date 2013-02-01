(defun copy-word (ARG)
  "Copy the next `ARG' words to kill-ring"
  (let ((beg (point)))
    (forward-word ARG)
    (copy-region-as-kill beg (point))))
(defun append-stat (search-string stat-name stats)
  "`search-string' should place point at a number associated with
`stat-name' convert that string to a number and append it to
a-list `stats'"
  (goto-char (point-min))
  (when (> (buffer-size) 1)  
    (search-forward search-string nil t nil)
    (copy-word 1)
    (cons (cons stat-name (string-to-number (current-kill 0 t))) stats)))
(defun append-charge (stats)
  (goto-char (point-min))
  (when (> (buffer-size) 1)  (search-forward "Mean Jet Charge: " nil t nil)
	(copy-word 2)
	(forward-char 5)
	(copy-word 2)
	(let ((mean (string-to-number (current-kill 1 t)))
	      (stddev (string-to-number (current-kill 0 t))))
	  (cons (cons 'jet-charge (cons mean stddev)) stats))))
(defun extract-log-data (buf)
  "Returns a a-list of cutflow and charge measurement"
    (let ((current-buf (current-buffer)) 
	  stats)
    (switch-to-buffer buf)
      (setq stats (append-stat "Inclusive | " 'inclusive stats))
      (setq stats (append-stat "Found W   | " 'found-w stats))
      (setq stats (append-stat ">1 Jet    | " 'greater-one-jet stats))
      (setq stats (append-stat "Fiducial  | " 'fiducial stats))
      (setq stats (append-charge stats))
      ;(kill-buffer (current-buffer))
      (switch-to-buffer current-buf) stats))
(defun get-files (dir suffix &optional split-func split-func-args)
  "Return a list of files from `dir' containing regexp `suffix'
  and remove those which do not pass `split-func'"
  (let ((files (directory-files dir t suffix)))
    (when (functionp split-func) 
      (dolist (fname files)
	(unless (funcall split-func fname split-func-args) (delete fname files))))
    files))
(defun summarize-condor-results (dir suffix &optional split-func split-func-args)
 "Iterate over `dir' containing output logs and summarize the
total result of a condor job where `suffix' is a regexp matching
log files to process"
  (let ((n 0)
       (jet-charge 0)
       (jet-charge-err 0)
       (fiducial 0) 
       (greater-one-jet 0) 
       (found-w 0) 
       (inclusive 0)
       (cur-buf (current-buffer))) 
    (save-excursion  (dolist (stat-list (mapcar (lambda (file)
						  (when (file-readable-p file) 
						    (extract-log-data (find-file file)))) 
						(get-files dir suffix split-func split-func-args)))
		       (unless (null stat-list) 
			 (incf n)
			 (setq jet-charge (+ jet-charge (cadr (assoc 'jet-charge stat-list))))
			 (let ((err (cddr (assoc 'jet-charge stat-list)))) 
			   (setq jet-charge-err (+ jet-charge-err (* err err))))
			 (setq fiducial (+ fiducial (cdr (assoc 'fiducial stat-list))))
			 (setq greater-one-jet (+ greater-one-jet (cdr (assoc 'greater-one-jet stat-list))) )
			 (setq found-w (+ found-w (cdr (assoc 'found-w stat-list))))
			 (setq inclusive (+ inclusive (cdr (assoc 'inclusive stat-list)))) ))) 
    (switch-to-buffer cur-buf)
    (list (cons 'jet-charge (cons (/ jet-charge n) (sqrt (/ jet-charge-err n)))) 
	  (cons 'inclusive inclusive)
	  (cons 'found-w found-w) 
	  (cons 'greater-one-jet greater-one-jet) 
	  (cons 'fiducial fiducial))))
(defun find-tune-text (fname tune-string)
  "Return nil if `tune-string' doesn't exist in `fname'"
  (let ((buf (current-buffer))
	res)  
    (switch-to-buffer (find-file fname))
    (setq res (search-forward tune-string nil t nil))
    (switch-to-buffer buf) res))

(defun split-by-n (fname n)
  "Take a string of the form \"name.Cluster.Process\" where Cluster
  and Process are numbers. Return fname if Process < n, nil else"
  (if (< (string-to-number (car (last (split-string fname "\\.")))) n)
      fname nil))

(defun print-condor-summary (dir suffix &optional gen-name tune split-func split-func-args)
(let ((data (summarize-condor-results dir suffix split-func split-func-args)))
  (when gen-name (insert (format "\nGenerator: %s" gen-name)))
  (if tune (insert (format ", %s\n" tune))
    (insert "\n"))
  (insert (format "Q=%.3f±%.3f \n" (cadar data) (cddar data)))
  (dolist (pair (cdr data))
    (insert (format "| %-15s | %-6d |\n" (symbol-name (car pair)) (cdr pair))))
  (insert (format "| %-15s | %-6.3f |\n" "Efficiency " 
		  (/ (cdr (assoc 'fiducial data)) 
		     (+ (cdr (assoc 'inclusive data)) 0.0))))))

(defun clean-buffer-list-regexp (regexp)
"Remove all buffers matching `regexp' without prompting."
(mapcar (lambda (buf)
	  (interactive)
	  (let ((buffer-modified-p nil))
	    (when (string-match regexp (buffer-name buf))
	      (kill-buffer (current-buffer))))) (buffer-list)))

(print-condor-summary "/ssh:dmb60@atl008:~/rivet/Analysis/rivet-jet-charge/MonteCarloParams/Herwig++" "\\.log" "Herwig++")

(print-condor-summary "/ssh:dmb60@atl008:~/logs/rivet/Sherpa" "^out.*" "Sherpa")
(print-condor-summary
 "/ssh:dmb60@atl008:~/logs/rivet/Pythia6" "^out.*" "Pythia 6" "Perugia 2011, MSTW LO**"
 (lambda (fname x)
   (find-tune-text fname "355")))

(print-condor-summary "/ssh:dmb60@atl008:~/logs/rivet/Pythia6" "^out.*" "Pythia 6" "Perugia 2011, CTEQ6L1" (lambda (fname x)
   (find-tune-text fname "356")))

(print-condor-summary "/ssh:dmb60@atl008:~/logs/rivet/Pythia8" "^out.*" "Pythia 8" (lambda (fname)
										     (find-tune-text fname "MSTW2008lo68cl")))

