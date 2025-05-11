import json
from pathlib import Path

STATE_FILE = Path('state.json')

DEFAULT_STATE = {
    'mode': 'manual',  
    'locked': True     
}

def load_state():
    """Загрузить состояние из файла (или вернуть default)."""
    if STATE_FILE.exists():
        try:
            return json.loads(STATE_FILE.read_text())
        except json.JSONDecodeError:
            pass
    return DEFAULT_STATE.copy()

def save_state(state):
    """Сохранить состояние в файл."""
    STATE_FILE.write_text(json.dumps(state))

def get_mode():
    return load_state().get('mode', DEFAULT_STATE['mode'])

def set_mode(mode):
    state = load_state()
    state['mode'] = mode
    save_state(state)

def toggle_mode():
    """Переключить режим между manual и auto и вернуть новый режим."""
    state = load_state()
    state['mode'] = 'auto' if state.get('mode') == 'manual' else 'manual'
    save_state(state)
    return state['mode']

def is_locked():
    return load_state().get('locked', DEFAULT_STATE['locked'])

def set_locked(locked):
    state = load_state()
    state['locked'] = locked
    save_state(state)
