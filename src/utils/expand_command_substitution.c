/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_substitution.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 17:12:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 17:12:51 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_closing_paren(char *str, int start)
{
	int	paren_count;
	int	i;

	paren_count = 1;
	i = start;
	while (str[i] && paren_count > 0)
	{
		if (str[i] == '(')
			paren_count++;
		else if (str[i] == ')')
			paren_count--;
		i++;
	}
	if (paren_count == 0)
		return (i - 1);
	return (-1);
}

static char	*read_command_output(int pipefd[2])
{
	char	buffer[1024];
	int		bytes_read;
	char	*result;

	close(pipefd[1]);
	bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
	close(pipefd[0]);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		result = ft_strdup(buffer);
	}
	else
		result = ft_strdup("");
	return (result);
}

char	*execute_command_substitution(char *command, t_minishell *shell)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (ft_strdup(""));
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		process_input(command, shell);
		exit(shell->last_exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		return (read_command_output(pipefd));
	}
	else
		return (ft_strdup(""));
}

static char	*replace_command_substitution(char *result, char *command_output,
	int start, int end)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*final;

	before = ft_substr(result, 0, start);
	after = ft_strdup(result + end + 1);
	temp = ft_strjoin(before, command_output);
	final = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	return (final);
}

void	process_command_substitution(char **result, int *i, t_minishell *shell)
{
	int		closing_paren;
	char	*command;
	char	*command_output;
	char	*new_result;
	int		command_len;

	closing_paren = find_closing_paren(*result, *i + 2);
	if (closing_paren == -1)
	{
		*i += 2;
		return ;
	}
	command = ft_substr(*result, *i + 2, closing_paren - *i - 2);
	command_output = execute_command_substitution(command, shell);
	new_result = replace_command_substitution(*result, command_output,
			*i, closing_paren);
	command_len = ft_strlen(command_output);
	free(command);
	free(command_output);
	free(*result);
	*result = new_result;
	*i += command_len;
}
