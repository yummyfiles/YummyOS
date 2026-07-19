# YummyOS default .zshrc

# ── History ───────────────────────────────────────────────────────────────────
HISTFILE=~/.zsh_history
HISTSIZE=10000
SAVEHIST=10000
setopt appendhistory
setopt sharehistory
setopt hist_ignore_space
setopt hist_ignore_dups
setopt hist_reduce_blanks

# ── Completion ────────────────────────────────────────────────────────────────
autoload -Uz compinit
compinit

zstyle ':completion:*' menu select
zstyle ':completion:*' matcher-list 'm:{a-zA-Z}={A-Za-z}' 'r:|=*' 'l:|=* r:|=*'
zstyle ':completion:*' list-colors ${(s.:.)LS_COLORS}

# ── Prompt ────────────────────────────────────────────────────────────────────
autoload -Uz vcs_info
precmd() { vcs_info }

zstyle ':vcs_info:git:*' formats '(%b)'
zstyle ':vcs_info:*' enable git

setopt PROMPT_SUBST
PROMPT='%F{#FF6B9D}yum%f %F{#4ECDC4}%~%f %F{#FFE66D}${vcs_info_msg_0_}%f %# '

# ── Aliases ───────────────────────────────────────────────────────────────────
alias ls='ls --color=auto'
alias ll='ls -lah'
alias la='ls -lAh'
alias grep='grep --color=auto'
alias diff='diff --color=auto'
alias pacman='pacman --color=auto'

# ── Key bindings ──────────────────────────────────────────────────────────────
bindkey -e
bindkey '^[[A' history-search-backward
bindkey '^[[B' history-search-forward
bindkey '^[[H' beginning-of-line
bindkey '^[[F' end-of-line
bindkey '^[[3~' delete-char

# ── PATH ──────────────────────────────────────────────────────────────────────
export PATH="$HOME/.local/bin:$PATH"
export EDITOR='nvim'
export VISUAL='nvim'

# ── Wayland / XDG ─────────────────────────────────────────────────────────────
export XDG_CURRENT_DESKTOP=KDE
export XDG_SESSION_TYPE=wayland
export QT_QPA_PLATFORM=wayland
export GDK_BACKEND=wayland

# ── Starship prompt (if installed) ────────────────────────────────────────────
if command -v starship &>/dev/null; then
    eval "$(starship init zsh)"
fi

# ── Zoxide (if installed) ─────────────────────────────────────────────────────
if command -v zoxide &>/dev/null; then
    eval "$(zoxide init zsh)"
fi

# ── fzf (if installed) ────────────────────────────────────────────────────────
if command -v fzf &>/dev/null; then
    source <(fzf --zsh)
fi