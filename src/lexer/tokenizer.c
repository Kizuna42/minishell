/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:35:34 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

static char	*extract_word(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && !is_whitespace(str[*i]) && !is_operator(str[*i])
		&& !is_quote(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static void	process_operator_token(char *input, int *i, t_token **tokens)
{
	t_token	*token;
	int		advance;

	token = create_token(get_operator_type(input + *i, &advance),
			ft_substr(input, *i, advance));
	*i += advance;
	add_token(tokens, token);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	t_token	*token;
	char	*value;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_quote(input[i]))
			value = extract_quoted_string(input, &i);
		else if (is_operator(input[i]))
		{
			process_operator_token(input, &i, &tokens);
			continue ;
		}
		else
			value = extract_word(input, &i);
		token = create_token(TOKEN_WORD, value);
		add_token(&tokens, token);
	}
	return (tokens);
}
