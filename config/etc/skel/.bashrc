# YummyOS default .bashrc

# ── History ───────────────────────────────────────────────────────────────────
HISTSIZE=10000
HISTFILESIZE=20000
HISTCONTROL=ignoreboth:erasedups
shopt -s histappend
shopt -s checkwinsize

# ── Prompt ────────────────────────────────────────────────────────────────────
__yummy_ps1() {
    local exit_code=$?
    local prompt=""

    # Color: red if last command failed, green otherwise
    if [ $exit_code -eq 0 ]; then
        prompt+="\[\e[38;2;255;107;157m\]yum\[\e[0m\] "
    else
        prompt+="\[\e[31m\]yum\[\e[0m\] "
    fi

    prompt+="\[\e[38;2;78;205;196m\]\w\[\e[0m\] "

    # Git branch
    if command -v git &>/dev/null && git rev-parse --git-dir &>/dev/null 2>&1; then
        local branch=$(git branch --show-current 2>/dev/null)
        if [ -n "$branch" ]; then
            prompt+="\[\e[38;2;255;230;109m\]($branch)\[\e[0m\] "
        fi
    fi

    prompt+="% "

    PS1="$prompt"
}

PROMPT_COMMAND="__yummy_ps1"

# ── Aliases ───────────────────────────────────────────────────────────────────
alias ls='ls --color=auto'
alias ll='ls -lah'
alias la='ls -lAh'
alias grep='grep --color=auto'
alias diff='diff --color=auto'
alias pacman='pacman --color=auto'
alias update='sudo pacman -Syu'
alias install='sudo pacman -S'
alias search='pacman -Ss'
alias remove='sudo pacman -Rns'

# ── PATH ──────────────────────────────────────────────────────────────────────
export PATH="$HOME/.local/bin:$PATH"
export EDITOR='nvim'
export VISUAL='nvim'

# ── Wayland / XDG ─────────────────────────────────────────────────────────────
export XDG_CURRENT_DESKTOP=KDE
export XDG_SESSION_TYPE=wayland
export QT_QPA_PLATFORM=wayland
export GDK_BACKEND=wayland

# ── Bash completions ──────────────────────────────────────────────────────────
[ -f /usr/share/bash-completion/bash_completion ] && . /usr/share/bash-completion/bash_completion

# ── Starship prompt (if installed) ────────────────────────────────────────────
if command -v starship &>/dev/null; then
    eval "$(starship init bash)"
fi

# ── Zoxide (if installed) ─────────────────────────────────────────────────────
if command -v zoxide &>/dev/null; then
    eval "$(zoxide init bash)"
fi

# ── fzf (if installed) ────────────────────────────────────────────────────────
if command -v fzf &>/dev/null; then
    source <(fzf --bash)
fi